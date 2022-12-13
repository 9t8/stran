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

		const datum &cddr(*cdr.cdr);
		assert(typeid(cddr) == typeid(empty_list) && "too many arguments");

		const datum &temp3(*args_list.car);
		assert(typeid(temp3) == typeid(identifier) &&
			   "first argument must be an identifier");
		const identifier &variable(dynamic_cast<const identifier &>(*args_list.car));

		env[variable.name] = cdr.car->eval(env);

		return std::make_shared<empty_list>();
	}
	);

	auto lambda(
	[](const p_datum &args, environment &env) {
		const datum &temp(*args);
		assert(typeid(temp) == typeid(pair) &&
			   "malformed argument list (not enough arguments?)");
		const pair &args_list(dynamic_cast<const pair &>(temp));

		const datum &temp1(*args_list.cdr);
		assert(typeid(temp1) == typeid(pair) &&
			   "malformed argument list (not enough arguments?)");
		const pair &cdr(dynamic_cast<const pair &>(temp1));

		const pair *curr_formal(dynamic_cast<const pair *>(args_list.car.get()));

		std::vector<std::string> formals;
		while (curr_formal != nullptr) {
			const datum &formal_iden(*curr_formal->car);
			assert(typeid(formal_iden) == typeid(identifier) &&
				   "all formals must be identifiers (variadics are not supported)");

			formals.push_back(dynamic_cast<const identifier &>(formal_iden).name);
			curr_formal = dynamic_cast<const pair *>(curr_formal->cdr.get());
		}
		return std::make_shared<procedure>(formals, cdr.car, false);
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
