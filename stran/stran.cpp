#include <iostream>

#include "parse.h"

using namespace stran;

static sp<datum> lambda(const pair &args_list, const sp<env> &curr_env) {
	std::vector<std::string> formals;

	sp<iden> variadic_iden(std::dynamic_pointer_cast<iden>(args_list.car));
	sp<pair> curr_formal(std::dynamic_pointer_cast<pair>(args_list.car));
	while (curr_formal) {
		const datum &next_formal(*curr_formal->car);
		assert(typeid(next_formal) == typeid(iden) &&
			   "all formals must be identifiers (variadics are not supported)");

		formals.push_back(dynamic_cast<const iden &>(next_formal).name);

		variadic_iden = std::dynamic_pointer_cast<iden>(curr_formal->cdr);
		curr_formal = std::dynamic_pointer_cast<pair>(curr_formal->cdr);
	}
	if (variadic_iden) {
		formals.push_back(variadic_iden->name);
	}

	return std::make_shared<closure>(formals, static_cast<bool>(variadic_iden),
			std::dynamic_pointer_cast<pair>(args_list.cdr), curr_env);
}

static sp<datum> define(const pair &args_list, const sp<env> &curr_env) {
	const datum &temp(*args_list.cdr);
	assert(typeid(temp) == typeid(pair) && "not enough arguments");
	const pair &cdr(dynamic_cast<const pair &>(*args_list.cdr));

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
						 lambda( {formals.cdr, args_list.cdr}, curr_env));
	}

	return nullptr;
}

static sp<datum> quote(const pair &args_list, const sp<env> &) {
	assert(!args_list.cdr && "too many arguments");

	return args_list.car;
}

int main(int, const char *[]) {
	tok_list toks(lex(std::cin));

	std::cout << "===-- tokens --===\n";
	for (size_t i(0); i < toks.size(); ++i) {
		std::cout << " " << to_string(toks[i]);
	}
	std::cout << "\n";

	std::vector<sp<datum>> tree;
	for (size_t i(0); i < toks.size();) {
		tree.push_back(parse_datum(toks, i));
	}

	std::cout << "\n===-- tree --===\n";
	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << to_string(tree[i]) << "\n";
	}
	std::cout << "\n===-- output --===" << std::endl;

	const sp<env> top_level(std::make_shared<env>(nullptr));
	top_level->define("lambda", std::make_shared<native_func>(lambda));
	top_level->define("define", std::make_shared<native_func>(define));
	top_level->define("quote", std::make_shared<native_func>(quote));

	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << to_string(eval(tree[i], top_level)) << std::endl;
	}
}
