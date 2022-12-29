#include "closure.h"

using namespace stranx;

sp<datum> closure::call(const sp<datum> &args, const sp<env> &p_e) const {
	const sp<env> eval_env(std::make_shared<env>(context));

	sp<pair> curr_arg(std::dynamic_pointer_cast<pair>(args));

	for (size_t i(0); i + 1 < formals.size(); ++i) {
		eval_env->define(formals[i], next(curr_arg)->eval(p_e));
	}

	if (!variadic) {
		if (!formals.empty()) {
			eval_env->define(formals.back(), next(curr_arg)->eval(p_e));
		}
		assert(!curr_arg && "too many arguments");

		return eval_body(eval_env);
	}

	if (!curr_arg) {
		eval_env->define(formals.back(), std::make_shared<emptyl>());
		return eval_body(eval_env);
	}

	sp<pair> tail(std::make_shared<pair>(next(curr_arg)->eval(p_e)));
	eval_env->define(formals.back(), tail);
	while (curr_arg) {
		sp<pair> new_tail(std::make_shared<pair>(next(curr_arg)->eval(p_e)));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return eval_body(eval_env);
}

sp<datum> closure::eval_body(const sp<env> &eval_env) const {
	sp<pair> exprs(body);
	sp<datum> result(next(exprs)->eval(eval_env));
	while (exprs) {
		result = next(exprs)->eval(eval_env);
	}
	return result;
}

sp<datum> lambda::call(const sp<datum> &args, const sp<env> &p_e) const {
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list (not enough arguments?)");
	const pair &args_list(dynamic_cast<const pair &>(temp));

	std::vector<std::string> formals;

	sp<iden> variadic_iden(std::dynamic_pointer_cast<iden>(args_list.car));
	sp<pair> curr_formal(std::dynamic_pointer_cast<pair>(args_list.car));
	while (curr_formal) {
		variadic_iden = std::dynamic_pointer_cast<iden>(curr_formal->cdr);

		const datum &formal_iden(*next(curr_formal));
		assert(typeid(formal_iden) == typeid(iden) &&
			   "all formals must be identifiers (variadics are not supported)");

		formals.push_back(dynamic_cast<const iden &>(formal_iden).name);
	}
	if (variadic_iden) {
		formals.push_back(variadic_iden->name);
	}

	const sp<pair> body(std::dynamic_pointer_cast<pair>(args_list.cdr));
	assert(body && "invalid procedure body");

	return std::make_shared<closure>(formals, variadic_iden.get(), body, p_e);
}

sp<datum> define::call(const sp<datum> &args, const sp<env> &p_e) const {
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list (not enough arguments?)");
	const pair &args_list(dynamic_cast<const pair &>(temp));

	const datum &temp1(*args_list.cdr);
	assert(typeid(temp1) == typeid(pair) && "malformed argument list (not enough arguments?)");
	const pair &cdr(dynamic_cast<const pair &>(temp1));

	const datum &car(*args_list.car);

	if (typeid(car) == typeid(iden)) {
		const datum &cddr(*cdr.cdr);
		assert(typeid(cddr) == typeid(emptyl) && "too many arguments");

		p_e->define(dynamic_cast<const iden &>(*args_list.car).name, cdr.car->eval(p_e));
	} else {
		assert(typeid(car) == typeid(pair) && "first argument must be identifier or list");

		const pair &formals(dynamic_cast<const pair &>(*args_list.car));

		const datum &caar(*formals.car);
		assert(typeid(caar) == typeid(iden) && "procedure name must be an identifier");

		const sp<datum> lambda(p_e->find("lambda"));
		assert(dynamic_cast<const func *>(lambda.get()) &&
			   "lambda was redefined into an uncallable token");

		p_e->define(dynamic_cast<const iden &>(*formals.car).name,
					dynamic_cast<const func &>(*lambda).call(std::make_shared<pair>(
								formals.cdr, args_list.cdr), p_e));
	}

	return std::make_shared<emptyl>();
}
