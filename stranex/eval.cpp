#include "eval.h"

#include "token.h"

#include <ostream>

p_datum define(environment &env, const syntax_tree &args) {
	const datum &variable(*args[0]);
	assert(typeid(variable) == typeid(identifier) && "first argument must be an idenifier");

	env[dynamic_cast<const identifier &>(variable).name] = args[1]->eval(env);
	return p_datum(new list);
}

p_datum lambda(environment &env, const syntax_tree &args) {
	const datum &formals_list(*args[0]);
	// todo: accept varargs
	assert(typeid(formals_list) == typeid(list) && "first argument must be a list");

	const syntax_tree &formals_tree(dynamic_cast<const list &>(formals_list).elements);

	std::vector<std::string> formals(formals_tree.size());
	for (size_t i(0); i < formals.size(); ++i) {
		const datum &formal(*formals_tree[i]);
		assert(typeid(formal) == typeid(identifier) && "all formals must be identifiers");

		formals[i] = dynamic_cast<const identifier &>(formal).name;
	}

	return p_datum(new procedure(formals, args[1]));
}

void eval(syntax_tree &tree, std::ostream &os) {
	environment env;
	env["define"] = p_datum(
						new native_function( {"variable", "expression"}, define)
					);
	env["lambda"] = p_datum(
						new native_function( {"formals", "body"}, lambda)
					);

	for (size_t i(0); i < tree.size(); ++i) {
		os << *tree[i]->eval(env) << std::endl;
	}
}
