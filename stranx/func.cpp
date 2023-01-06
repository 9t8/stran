#include "func.h"

using namespace stranx;

bool pair::stringify_into_lists(true);

pair::operator std::string() const {
	if (!stringify_into_lists) {
		return "(" + static_cast<std::string>(*car) + " . " + static_cast<std::string>(*cdr)
			   + ")";
	}

	std::ostringstream oss;
	oss << "(" << *car;

	sp<datum> p_last(cdr);
	for (sp<pair> curr_pair(std::dynamic_pointer_cast<pair>(cdr)); curr_pair;) {
		p_last = curr_pair->cdr;
		oss << " " << *next(curr_pair);
	}

	const datum &last(*p_last);
	if (typeid(last) != typeid(emptyl)) {
		oss << " . " << last;
	}
	oss << ")";

	return oss.str();
}

const sp<datum> &stranx::next(sp<pair> &exprs) {
	assert(exprs && "invalid expression list (not enough arguments?)");

	const sp<datum> &result(exprs->car);
	exprs = std::dynamic_pointer_cast<pair>(exprs->cdr);
	return result;
}

sp<datum> closure::call(const sp<datum> &args, const sp<env> &curr_env) const {
	const sp<env> eval_env(std::make_shared<env>(context));

	const auto eval_body([&]() -> sp<datum> {
		sp<pair> exprs(body);
		sp<datum> result(safe_eval(next(exprs), eval_env));
		while (exprs) {
			result = safe_eval(next(exprs), eval_env)
					 ;
		}
		return result;
	});

	sp<pair> curr_arg(std::dynamic_pointer_cast<pair>(args));

	for (size_t i(0); i + 1 < formals.size(); ++i) {
		eval_env->define(formals[i], safe_eval(next(curr_arg), curr_env));
	}

	if (!variadic) {
		if (!formals.empty()) {
			eval_env->define(formals.back(), safe_eval(next(curr_arg), curr_env));
		}
		assert(!curr_arg && "too many arguments");

		return eval_body();
	}

	if (!curr_arg) {
		eval_env->define(formals.back(), std::make_shared<emptyl>());
		return eval_body();
	}

	sp<pair> tail(std::make_shared<pair>(safe_eval(next(curr_arg), curr_env)));
	eval_env->define(formals.back(), tail);
	while (curr_arg) {
		sp<pair> new_tail(std::make_shared<pair>(safe_eval(next(curr_arg), curr_env)));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return eval_body();
}
