#include "lexer.h"
#include "procedure.h"

#include <iostream>
#include <vector>

static const auto lambda(
[](const p_datum &args, const_p_env &env) {
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list (not enough arguments?)");
	const pair &args_list(dynamic_cast<const pair &>(temp));

	std::vector<std::string> formals;

	std::shared_ptr<identifier> variadic_iden(
		std::dynamic_pointer_cast<identifier>(args_list.car));
	p_pair curr_formal(std::dynamic_pointer_cast<pair>(args_list.car));
	while (curr_formal) {
		variadic_iden = std::dynamic_pointer_cast<identifier>(curr_formal->cdr);

		const datum &formal_iden(*next(curr_formal));
		assert(typeid(formal_iden) == typeid(identifier) &&
			   "all formals must be identifiers (variadics are not supported)");

		formals.push_back(dynamic_cast<const identifier &>(formal_iden).name);
	}
	if (variadic_iden) {
		formals.push_back(variadic_iden->name);
	}

	const p_pair body(std::dynamic_pointer_cast<pair>(args_list.cdr));
	assert(body && "invalid procedure body");

	return std::make_shared<procedure>(formals, variadic_iden.get(), body, env);
}
);

static const auto define(
[](const p_datum &args, const_p_env &env) {
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list (not enough arguments?)");
	const pair &args_list(dynamic_cast<const pair &>(temp));

	const datum &temp1(*args_list.cdr);
	assert(typeid(temp1) == typeid(pair) && "malformed argument list (not enough arguments?)");
	const pair &cdr(dynamic_cast<const pair &>(temp1));

	const datum &car(*args_list.car);

	if (typeid(car) == typeid(identifier)) {
		const datum &cddr(*cdr.cdr);
		assert(typeid(cddr) == typeid(empty_list) && "too many arguments");

		env->define(dynamic_cast<const identifier &>(*args_list.car).name, cdr.car->eval(env));
	} else {
		assert(typeid(car) == typeid(pair) && "first argument must be identifier or list");

		const pair &formals(dynamic_cast<const pair &>(*args_list.car));

		const datum &caar(*formals.car);
		assert(typeid(caar) == typeid(identifier) && "procedure name must be an identifier");

		const p_datum lambda(env->find("lambda"));
		assert(dynamic_cast<const function *>(lambda.get()) &&
			   "lambda was redefined into an uncallable object");

		env->define(dynamic_cast<const identifier &>(*formals.car).name,
					dynamic_cast<const function &>(*lambda).call(std::make_shared<pair>(
								formals.cdr, args_list.cdr), env));
	}

	return std::make_shared<empty_list>();
}
);

p_datum parse_next(const token_list &tokens, size_t &idx) {
	static const auto get_next_token_type([&]() -> const std::type_info & {
		assert(idx < tokens.size() &&
		"expected a token but none found (too many opening parens?)");

		object &next_token(*tokens[idx]);
		return typeid(next_token);
	});

	if (get_next_token_type() == typeid(begin_list)) {
		++idx;
		if (get_next_token_type() == typeid(end_list)) {
			++idx;
			return std::make_shared<empty_list>();
		}

		p_pair p(std::make_shared<pair>(parse_next(tokens, idx)));
		p_datum start(p);
		while (get_next_token_type() != typeid(end_list)) {
			if (get_next_token_type() == typeid(dot)) {
				p->cdr = parse_next(tokens, ++idx);
				assert(get_next_token_type() == typeid(end_list) &&
					   "malformed improper list (misplaced dot token)");
				break;
			}
			p_pair p_new(std::make_shared<pair>(parse_next(tokens, idx)));
			p->cdr = p_new;
			p = p_new;
		}
		++idx;

		return start;
	}

	p_datum p(std::dynamic_pointer_cast<datum>(tokens.at(idx++)));
	assert(p && "malformed token list (tried to parse an unparsable token)");

	return p;
}

int main(int, const char *[]) {
	token_list tokens(lexer(std::cin).lex());

	std::cout << "===-- tokens --===" << std::endl;
	if (!tokens.empty()) {
		for (size_t i(0); i < tokens.size() - 1; ++i) {
			std::cout << *tokens[i] << " " << std::flush;
		}
		std::cout << *tokens.back() << std::endl;
	}

	std::vector<p_datum> tree;
	for (size_t i(0); i < tokens.size();) {
		tree.push_back(parse_next(tokens, i));
	}

	std::cout << "\n===-- tree --===" << std::endl;
	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << "  " << *tree[i] << std::endl;
	}

	const_p_env env(std::make_shared<environment>(nullptr));
	env->define("lambda", std::make_shared<native_function<decltype(lambda)>>(lambda));
	env->define("define", std::make_shared<native_function<decltype(define)>>(define));

	std::cout << "\n===-- output --===" << std::endl;
	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << *tree[i]->eval(env) << std::endl;
	}
}
