#include "eval.h"

void eval(std::vector<p_datum> &tree, std::ostream &os) {
	static const auto lambda(
	[](const p_datum &args, const p_env &env) {
		const datum &temp(*args);
		assert(typeid(temp) == typeid(pair) &&
			   "malformed argument list (not enough arguments?)");
		const pair &args_list(dynamic_cast<const pair &>(temp));

		std::vector<std::string> formals;

		std::shared_ptr<identifier> variadic_iden(
			std::dynamic_pointer_cast<identifier>(args_list.car));
		const pair *curr_formal(dynamic_cast<const pair *>(args_list.car.get()));
		while (curr_formal) {
			variadic_iden = std::dynamic_pointer_cast<identifier>(curr_formal->cdr);

			const datum &formal_iden(*next(curr_formal));
			assert(typeid(formal_iden) == typeid(identifier) &&
				   "all formals must be identifiers (variadics are not supported)");

			formals.push_back(dynamic_cast<const identifier &>(formal_iden).name);
		}

		const datum &body(*args_list.cdr);
		assert(typeid(body) == typeid(pair) && "invalid procedure body");

		if (variadic_iden) {
			formals.push_back(variadic_iden->name);
		}
		return std::make_shared<procedure>(formals, variadic_iden.get(), args_list.cdr, env);
	}
	);

	static const auto define(
	[](const p_datum &args, const p_env &env) {
		const datum &temp(*args);
		assert(typeid(temp) == typeid(pair) &&
			   "malformed argument list (not enough arguments?)");
		const pair &args_list(dynamic_cast<const pair &>(temp));

		const datum &temp1(*args_list.cdr);
		assert(typeid(temp1) == typeid(pair) &&
			   "malformed argument list (not enough arguments?)");
		const pair &cdr(dynamic_cast<const pair &>(temp1));

		const datum &car(*args_list.car);

		if (typeid(car) == typeid(identifier)) {
			const datum &cddr(*cdr.cdr);
			assert(typeid(cddr) == typeid(empty_list) && "too many arguments");

			(*env)[dynamic_cast<const identifier &>(*args_list.car).name] = cdr.car->eval(env);
		} else {
			assert(typeid(car) == typeid(pair) && "first argument must be identifier or list");

			const pair &formals(dynamic_cast<const pair &>(*args_list.car));

			const datum &caar(*formals.car);
			assert(typeid(caar) == typeid(identifier) &&
				   "procedure name must be an identifier");

			(*env)[dynamic_cast<const identifier &>(*formals.car).name] =
				call(env->at("lambda"), std::make_shared<pair>(formals.cdr, args_list.cdr),
					 env);
		}

		return std::make_shared<empty_list>();
	}
	);

	p_env env(std::make_shared<p_env::element_type>(
	std::initializer_list<p_env::element_type::value_type> {
		{"define", std::make_shared<native_function<decltype(define)>>(define)},
		{"lambda", std::make_shared<native_function<decltype(lambda)>>(lambda)}
	}));

	for (size_t i(0); i < tree.size(); ++i) {
		os << *tree[i]->eval(env) << std::endl;
	}
}
