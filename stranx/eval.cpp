#include "eval.h"

#include "token.h"

#include <ostream>

void eval(std::vector<p_datum> &syntax_tree, std::ostream &os) {
	environment env( {
		{
			"define", p_datum(new native_function(
			[](environment &env, const p_datum &args) {
				const datum &variable(*args[0]);
				assert(typeid(variable) == typeid(identifier) &&
					   "first argument must be an identifier");

				env[dynamic_cast<const identifier &>(variable).name] = args[1]->eval(env);
				return p_datum(new empty_list);
			}
			))
		}, {
			"lambda", p_datum(new native_function(
			[](environment &env, const p_datum &args) {
				// turn args into vector while verifying them
				const datum &formals_datum(*args[0]);
				// todo: accept varargs
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

				return p_datum(new procedure(formals, args[1]));
			}
			))
		}
	});

	for (size_t i(0); i < syntax_tree.size(); ++i) {
		os << *syntax_tree[i]->eval(env) << std::endl;
	}
}
