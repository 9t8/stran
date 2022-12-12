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
		}/*, {
			"lambda", p_datum(new native_function(
			[](const p_datum &args, environment &env) {
				const pair *args_list(dynamic_cast<const pair *>(args.get()));
				assert(args_list != nullptr &&
					   "malformed argument list (not enough arguments?)");

				const datum &formals_datum(*args_list->car);
				assert(typeid(formals_datum) == typeid(list) &&
					   "first argument must be a list");

				const std::vector<p_datum> &formals_list(
					dynamic_cast<const list &>(formals_datum).elements);

				std::vector<std::string> formals(formals_list.size());
				for (size_t i(0); i < formals.size(); ++i) {
					const datum &formal(*formals_list[i]);
					assert(typeid(formal) == typeid(identifier) &&
						   "all formals must be identifiers");

					formals[i] = dynamic_cast<const identifier &>(formal).name;
				}

				return std::make_shared<procedure>(formals, args_list->cdr);
			}
			))
		}*/
	});

	for (size_t i(0); i < syntax_tree.size(); ++i) {
		os << *syntax_tree[i]->eval(env) << std::endl;
	}
}
