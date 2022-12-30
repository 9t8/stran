#include "closure.h"

using namespace stranx;

sp<datum> closure::call(const sp<datum> &args, const sp<env> &curr_env) const {
	const sp<env> eval_env(std::make_shared<env>(context));

	const auto eval_body([&]() -> sp<datum> {
		sp<pair> exprs(body);
		sp<datum> result(next(exprs)->eval(eval_env));
		while (exprs) {
			result = next(exprs)
					 ->eval(eval_env);
		}
		return result;
	});

	sp<pair> curr_arg(std::dynamic_pointer_cast<pair>(args));

	for (size_t i(0); i + 1 < formals.size(); ++i) {
		eval_env->define(formals[i], next(curr_arg)->eval(curr_env));
	}

	if (!variadic) {
		if (!formals.empty()) {
			eval_env->define(formals.back(), next(curr_arg)->eval(curr_env));
		}
		assert(!curr_arg && "too many arguments");

		return eval_body();
	}

	if (!curr_arg) {
		eval_env->define(formals.back(), std::make_shared<emptyl>());
		return eval_body();
	}

	sp<pair> tail(std::make_shared<pair>(next(curr_arg)->eval(curr_env)));
	eval_env->define(formals.back(), tail);
	while (curr_arg) {
		sp<pair> new_tail(std::make_shared<pair>(next(curr_arg)->eval(curr_env)));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return eval_body();
}
