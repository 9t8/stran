#include "eval.h"

#include "token.h"

#include <ostream>

void eval(std::vector<p_datum> &syntax_tree, std::ostream &os) {
	environment env( {
		{
			"define", p_datum(new native_function(
			[](const p_datum &args, environment &env) {
				assert(typeid(*args) == typeid(pair) &&
				"malformed argument list (not enough arguments?)");
				const pair &args_list(dynamic_cast<const pair &>(*args));

				assert(typeid(*args_list.cdr) == typeid(pair) &&
					   "malformed argument list (not enough arguments?)");
				const pair &cdr(dynamic_cast<const pair &>(*args_list.cdr));

				assert(typeid(*cdr.cdr) == typeid(empty_list) && "too many arguments");

				assert(typeid(*args_list.car) == typeid(identifier) &&
					   "first argument must be an identifier");
				const identifier &variable(dynamic_cast<const identifier &>(*args_list.car));

				env[variable.name] = cdr.car->eval(env);

				return std::make_shared<empty_list>();
			}
			))
		}, {
			"lambda", p_datum(new native_function(
			[](const p_datum &args, environment &env) {
				assert(typeid(*args) == typeid(pair) &&
				"malformed argument list (not enough arguments?)");
				const pair &args_list(dynamic_cast<const pair &>(*args));

				const pair *curr_formal(dynamic_cast<const pair *>(args_list.car.get()));

				std::vector<std::string> formals;
				while (curr_formal != nullptr) {
					assert(typeid(*curr_formal->car) == typeid(identifier) &&
						   "all formals must be identifiers");

					formals.push_back(dynamic_cast<const identifier &>(*curr_formal->car).name);
					curr_formal = dynamic_cast<const pair *>(curr_formal->cdr.get());
				}
				return std::make_shared<procedure>(formals, args_list.cdr, false);
			}
			))
		}
	});

	for (size_t i(0); i < syntax_tree.size(); ++i) {
		os << *syntax_tree[i]->eval(env) << std::endl;
	}
}
