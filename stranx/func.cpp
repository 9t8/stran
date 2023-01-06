#include "func.h"

using namespace stranx;

pair::operator std::string() const {
	std::ostringstream oss;

	if (car) {
		oss << *car;
	} else {
		oss << "()";
	}

	sp<datum> p_tail(cdr);
	for (sp<pair> curr_pair(std::dynamic_pointer_cast<pair>(p_tail)); curr_pair;) {
		p_tail = curr_pair->cdr;

		oss << " ";
		if (curr_pair->car) {
			oss << *curr_pair->car;
		} else {
			oss << "()";
		}
		curr_pair = std::dynamic_pointer_cast<pair>(p_tail);
	}

	if (p_tail) {
		oss << " . " << *p_tail;
	}

	return "(" + oss.str() + ")";
}

const sp<datum> &stranx::next(sp<pair> &exprs) {
	assert(exprs && "invalid expression list (not enough arguments?)");

	const sp<datum> &result(exprs->car);
	exprs = std::dynamic_pointer_cast<pair>(exprs->cdr);
	return result;
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
		sp<pair> exprs(body);
		sp<datum> result(eval(next(exprs), eval_env));
		while (exprs) {
			result = eval(next(exprs), eval_env)
					 ;
		}
		return result;
	});

	sp<pair> curr_arg(std::dynamic_pointer_cast<pair>(args));

	for (size_t i(0); i + 1 < formals.size(); ++i) {
		eval_env->define(formals[i], eval(next(curr_arg), curr_env));
	}

	if (!variadic) {
		if (!formals.empty()) {
			eval_env->define(formals.back(), eval(next(curr_arg), curr_env));
		}
		assert(!curr_arg && "too many arguments");

		return eval_body();
	}

	if (!curr_arg) {
		eval_env->define(formals.back(), nullptr);
		return eval_body();
	}

	sp<pair> tail(std::make_shared<pair>(eval(next(curr_arg), curr_env)));
	eval_env->define(formals.back(), tail);
	while (curr_arg) {
		sp<pair> new_tail(std::make_shared<pair>(eval(next(curr_arg), curr_env)));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return eval_body();
}
