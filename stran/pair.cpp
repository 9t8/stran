#include "pair.h"

using namespace stran;

pair::operator std::string() const {
	std::string result("(" + to_string(car));

	sp<datum> p_tail(cdr);
	for (sp<pair> curr_pair(std::dynamic_pointer_cast<pair>(p_tail)); curr_pair;) {
		p_tail = curr_pair->cdr;

		result += " " + to_string(curr_pair->car);
		curr_pair = std::dynamic_pointer_cast<pair>(p_tail);
	}
	if (p_tail) {
		result += " . " + to_string(p_tail);
	}

	return result + ")";
}

sp<datum> native_func::call(const sp<datum> &args, const sp<env> &curr_env) const {
	assert(args && "no nullary native functions because i am lazy");

	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list");

	return p_func(dynamic_cast<const pair &>(*args), curr_env);
}

sp<datum> closure::call(const sp<datum> &args, const sp<env> &curr_env) const {
	const sp<env> eval_env(std::make_shared<env>(context));

	const auto eval_body([&]() -> sp<datum> {
		sp<datum> result;
		sp<pair> exprs(body);
		while (exprs) {
			result = eval(exprs->car, eval_env)
					 ;
			exprs = std::dynamic_pointer_cast<pair>(exprs->cdr);
		}
		return result;
	});

	sp<pair> curr_arg(std::dynamic_pointer_cast<pair>(args));

	const auto eval_next([&]() -> sp<datum> {
		assert(curr_arg && "invalid expression list (not enough arguments?)");

		const sp<datum> result(eval(curr_arg->car, curr_env));
		curr_arg = std::dynamic_pointer_cast<pair>(curr_arg->cdr);

		return result;
	});

	for (size_t i(0); i + 1 < formals.size(); ++i) {
		eval_env->define(formals[i], eval_next());
	}

	if (!variadic) {
		if (!formals.empty()) {
			eval_env->define(formals.back(), eval_next());
		}
		assert(!curr_arg && "too many arguments");

		return eval_body();
	}

	if (!curr_arg) {
		eval_env->define(formals.back(), nullptr);
		return eval_body();
	}

	sp<pair> tail(std::make_shared<pair>(eval_next()));
	eval_env->define(formals.back(), tail);
	while (curr_arg) {
		sp<pair> new_tail(std::make_shared<pair>(eval_next()));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return eval_body();
}
