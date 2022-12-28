#include "closure.h"

p_datum closure::call(const p_datum &args, const p_env &) const {
	const p_env new_env(make_new_env(args));

	p_pair exprs(body);
	p_datum result(next(exprs)->eval(new_env));
	while (exprs) {
		result = next(exprs)->eval(new_env);
	}
	return result;
}

const p_env closure::make_new_env(const p_datum &args) const {
	const p_env new_env(std::make_shared<environment>(env));

	p_pair curr_arg(std::dynamic_pointer_cast<pair>(args));

	const auto next_arg([&]() {
		const p_datum new_arg(next(curr_arg)->eval(env));
		const std::shared_ptr<closure> new_closure(
			std::dynamic_pointer_cast<closure>(new_arg));
		if (new_closure) {
			// fixme: argument eval must happen in PAIR! not here!
			// print stuff and check if new_closure.env is parent of new_env (aka this.env)
			// it should be
			// if its not, we would either have to add a check here
			// or fix something in lambda.
			new_closure->set_env(new_env);
		}
		return new_arg;
	}
					   );

	if (formals.empty()) {
		assert(!curr_arg && "too many arguments");

		return new_env;
	}

	for (size_t i(0); i < formals.size() - 1; ++i) {
		new_env->define(formals[i], next_arg());
	}

	if (!variadic) {
		new_env->define(formals.back(), next_arg());
		assert(!curr_arg && "too many arguments");

		return new_env;
	}

	if (!curr_arg) {
		new_env->define(formals.back(), std::make_shared<empty_list>());
		return new_env;
	}

	p_pair tail(std::make_shared<pair>(next_arg()));
	new_env->define(formals.back(), tail);
	while (curr_arg) {
		p_pair new_tail(std::make_shared<pair>(next_arg()));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return new_env;
}

p_datum lambda::call(const p_datum &args, const p_env &env) const {
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

	return std::make_shared<closure>(formals, variadic_iden.get(), body, env);
}

p_datum define::call(const p_datum &args, const p_env &env) const {
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
