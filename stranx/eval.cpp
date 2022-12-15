#include "eval.h"

#include "token.h"

#include <ostream>

environment create_env() {
	auto define(
	[](const p_datum &args, environment &env) {
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

			env[dynamic_cast<const identifier &>(*args_list.car).name] = cdr.car->eval(env);
		} else {
			assert(typeid(car) == typeid(pair) && "first argument must be identifier or list");

			const pair &formals(dynamic_cast<const pair &>(*args_list.car));

			std::shared_ptr<pair> lambda_args(std::make_shared<pair>(
						formals.cdr, args_list.cdr));

			const datum &caar(*formals.car);
			assert(typeid(caar) == typeid(identifier) &&
				   "procedure name must be an identifier");

			env[dynamic_cast<const identifier &>(*formals.car).name] =
				call(find("lambda", env), lambda_args, env);
		}

		return std::make_shared<empty_list>();
	}
	);

	auto lambda(
	[](const p_datum &args, environment &env) {
		// todo: varargs
		
		const datum &temp(*args);
		assert(typeid(temp) == typeid(pair) &&
			   "malformed argument list (not enough arguments?)");
		const pair &args_list(dynamic_cast<const pair &>(temp));

		const pair *curr_formal(dynamic_cast<const pair *>(args_list.car.get()));

		std::vector<std::string> formals;
		while (curr_formal != nullptr) {
			const datum &formal_iden(*curr_formal->car);
			assert(typeid(formal_iden) == typeid(identifier) &&
				   "all formals must be identifiers (variadics are not supported)");

			formals.push_back(dynamic_cast<const identifier &>(formal_iden).name);
			curr_formal = dynamic_cast<const pair *>(curr_formal->cdr.get());
		}

		const datum &body(*args_list.cdr);
		assert(typeid(body) == typeid(pair) && "invalid procedure body");

		return std::make_shared<procedure>(formals, args_list.cdr, false);
	}
	);

	return environment( {
		{"define", std::make_shared<native_function<decltype(define)>>(define)},
		{"lambda", std::make_shared<native_function<decltype(lambda)>>(lambda)}
	});
}

void eval(std::vector<p_datum> &syntax_tree, std::ostream &os) {
	environment env(create_env());

	for (size_t i(0); i < syntax_tree.size(); ++i) {
		os << *syntax_tree[i]->eval(env) << std::endl;
	}
}
