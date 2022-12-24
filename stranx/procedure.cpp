#include "procedure.h"

p_datum procedure::call(const p_datum &args, const_p_env &) const {
	const_p_env new_env(make_new_env(args));

	p_pair exprs(body);
	p_datum result(next(exprs)->eval(new_env));
	while (exprs) {
		result = next(exprs)->eval(new_env);
	}
	return result;
}

const_p_env procedure::make_new_env(const p_datum &args) const {
	const_p_env new_env(std::make_shared<environment>(env));

	p_pair curr_arg(std::dynamic_pointer_cast<pair>(args));

	if (formals.empty()) {
		assert(!curr_arg && "too many arguments");

		return new_env;
	}

	for (size_t i(0); i < formals.size() - 1; ++i) {
		new_env->define(formals[i], next(curr_arg)->eval(env));
	}

	if (!variadic) {
		new_env->define(formals.back(), next(curr_arg)->eval(env));
		assert(!curr_arg && "too many arguments");

		return new_env;
	}

	if (!curr_arg) {
		new_env->define(formals.back(), std::make_shared<empty_list>());
		return new_env;
	}

	p_pair tail(std::make_shared<pair>(next(curr_arg)->eval(env)));
	new_env->define(formals.back(), tail);
	while (curr_arg) {
		p_pair new_tail(std::make_shared<pair>(next(curr_arg)->eval(env)));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return new_env;
}
