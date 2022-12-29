#include "closure.h"

using namespace stranx;

p_datum closure::call(const p_datum &args, const p_ctx &ctx) const {
	const p_ctx new_ctx(make_new_ctx(args, ctx));

	p_pair exprs(body);
	p_datum result(next(exprs)->eval(new_ctx));
	while (exprs) {
		result = next(exprs)->eval(new_ctx);
	}
	return result;
}

const p_ctx closure::make_new_ctx(const p_datum &args, const p_ctx &ctx) const {
	p_pair curr_arg(std::dynamic_pointer_cast<pair>(args));

	const p_ctx new_ctx(std::make_shared<context>(internal_ctx));

	if (formals.empty()) {
		assert(!curr_arg && "too many arguments");

		return new_ctx;
	}

	for (size_t i(0); i < formals.size() - 1; ++i) {
		new_ctx->define(formals[i], next(curr_arg)->eval(ctx));
	}

	if (!variadic) {
		new_ctx->define(formals.back(), next(curr_arg)->eval(ctx));
		assert(!curr_arg && "too many arguments");

		return new_ctx;
	}

	if (!curr_arg) {
		new_ctx->define(formals.back(), std::make_shared<emptyl>());
		return new_ctx;
	}

	p_pair tail(std::make_shared<pair>(next(curr_arg)->eval(ctx)));
	new_ctx->define(formals.back(), tail);
	while (curr_arg) {
		p_pair new_tail(std::make_shared<pair>(next(curr_arg)->eval(ctx)));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return new_ctx;
}

p_datum lambda::call(const p_datum &args, const p_ctx &ctx) const {
	const datum &temp(*args);
	assert(typeid(temp) == typeid(pair) && "malformed argument list (not enough arguments?)");
	const pair &args_list(dynamic_cast<const pair &>(temp));

	std::vector<std::string> formals;

	std::shared_ptr<iden> variadic_iden(std::dynamic_pointer_cast<iden>(args_list.car));
	p_pair curr_formal(std::dynamic_pointer_cast<pair>(args_list.car));
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

	const p_pair body(std::dynamic_pointer_cast<pair>(args_list.cdr));
	assert(body && "invalid procedure body");

	return std::make_shared<closure>(formals, variadic_iden.get(), body, ctx);
}

p_datum define::call(const p_datum &args, const p_ctx &ctx) const {
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

		ctx->define(dynamic_cast<const iden &>(*args_list.car).name, cdr.car->eval(ctx));
	} else {
		assert(typeid(car) == typeid(pair) && "first argument must be identifier or list");

		const pair &formals(dynamic_cast<const pair &>(*args_list.car));

		const datum &caar(*formals.car);
		assert(typeid(caar) == typeid(iden) && "procedure name must be an identifier");

		const p_datum lambda(ctx->find("lambda"));
		assert(dynamic_cast<const function *>(lambda.get()) &&
			   "lambda was redefined into an uncallable token");

		ctx->define(dynamic_cast<const iden &>(*formals.car).name,
					dynamic_cast<const function &>(*lambda).call(std::make_shared<pair>(
								formals.cdr, args_list.cdr), ctx));
	}

	return std::make_shared<emptyl>();
}
