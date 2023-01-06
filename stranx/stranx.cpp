#include "func.h"
#include "lex.h"

#include <iostream>

using namespace stranx;

static sp<datum> parse_datum(const tok_list &toks, size_t &idx) {
	const auto peek_next_type([&]() -> const std::type_info & {
		assert(idx < toks.size() && "expected more tokens but none found");

		tok &next_tok(*toks[idx]);
		return typeid(next_tok);
	});

	if (peek_next_type() == typeid(beginl)) {
		++idx;
		if (peek_next_type() == typeid(endl)) {
			++idx;
			return nullptr;
		}

		sp<pair> p(std::make_shared<pair>(parse_datum(toks, idx)));
		sp<datum> start(p);
		while (peek_next_type() != typeid(endl)) {
			if (peek_next_type() == typeid(dot)) {
				p->cdr = parse_datum(toks, ++idx);
				assert(peek_next_type() == typeid(endl) && "misplaced dot token");
				break;
			}
			sp<pair> p_new(std::make_shared<pair>(parse_datum(toks, idx)));
			p->cdr = p_new;
			p = p_new;
		}
		++idx;

		return start;
	}
	if (peek_next_type() == typeid(quote_tok)) {
		return std::make_shared<quote>(parse_datum(toks, ++idx));
	}

	sp<datum> p(std::dynamic_pointer_cast<datum>(toks.at(idx++)));
	assert(p && "tried to parse an unparsable token");

	return p;
}

static sp<datum> lambda(const sp<datum> &args, const sp<env> &curr_env) {
	assert(args && "not enough arguments");
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list");
	const pair &args_list(dynamic_cast<const pair &>(*args));

	std::vector<std::string> formals;

	sp<iden> variadic_iden(std::dynamic_pointer_cast<iden>(args_list.car));
	sp<pair> curr_formal(std::dynamic_pointer_cast<pair>(args_list.car));
	while (curr_formal) {
		variadic_iden = std::dynamic_pointer_cast<iden>(curr_formal->cdr);

		const datum &next_formal(*next(curr_formal));
		assert(typeid(next_formal) == typeid(iden) &&
			   "all formals must be identifiers (variadics are not supported)");

		formals.push_back(dynamic_cast<const iden &>(next_formal).name);
	}
	if (variadic_iden) {
		formals.push_back(variadic_iden->name);
	}

	const sp<pair> body(std::dynamic_pointer_cast<pair>(args_list.cdr));
	assert(body && "invalid procedure body");

	return std::make_shared<closure>(formals, static_cast<bool>(variadic_iden), body, curr_env);
}

static sp<datum> define(const sp<datum> &args, const sp<env> &curr_env) {
	assert(args && "not enough arguments");
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list");
	const pair &args_list(dynamic_cast<const pair &>(*args));

	const datum &temp1(*args_list.cdr);
	assert(typeid(temp1) == typeid(pair) && "not enough arguments");
	const pair &cdr(dynamic_cast<const pair &>(temp1));

	const datum &car(*args_list.car);

	if (typeid(car) == typeid(iden)) {
		assert(!cdr.cdr && "too many arguments");

		curr_env->define(dynamic_cast<const iden &>(*args_list.car).name,
						 eval(cdr.car, curr_env));
	} else {
		assert(typeid(car) == typeid(pair) && "first argument must be identifier or list");

		const pair &formals(dynamic_cast<const pair &>(*args_list.car));

		const datum &caar(*formals.car);
		assert(typeid(caar) == typeid(iden) && "procedure name must be an identifier");

		curr_env->define(dynamic_cast<const iden &>(*formals.car).name,
						 lambda(std::make_shared<pair>(formals.cdr, args_list.cdr), curr_env));
	}

	return nullptr;
}

static sp<datum> quote_func(const sp<datum> &args, const sp<env> &) {
	assert(args && "not enough arguments");
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list");
	const pair &args_list(dynamic_cast<const pair &>(*args));

	assert(!args_list.cdr && "too many arguments");

	return args_list.car;
}

int main(int, const char *[]) {
	tok_list toks(lex(std::cin));

	std::cout << "===-- tokens --===\n";
	for (size_t i(0); i < toks.size(); ++i) {
		std::cout << " " << *toks[i];
	}
	std::cout << "\n";

	std::vector<sp<datum>> tree;
	for (size_t i(0); i < toks.size();) {
		tree.push_back(parse_datum(toks, i));
	}

	std::cout << "\n===-- tree --===\n";
	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << *tree[i] << "\n";
	}
	std::cout << "\n===-- output --===" << std::endl;

	const sp<env> top_level(std::make_shared<env>(nullptr));
	top_level->define("lambda", std::make_shared<native_func>(lambda));
	top_level->define("define", std::make_shared<native_func>(define));
	top_level->define("quote", std::make_shared<native_func>(quote_func));

	for (size_t i(0); i < tree.size(); ++i) {
		sp<datum> result(eval(tree[i], top_level));
		if (result) {
			std::cout << *result << std::endl;
		} else {
			std::cout << "()" << std::endl;
		}
	}
}
