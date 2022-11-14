#include "interpret.h"

std::shared_ptr<object> interpret_next_expression(const std::unique_ptr<object> &p_ast,
		env_type &env) {
	/*if (typeid(*p_ast) == typeid(list)) {
		return "LIST\n";
	}*/
	return std::shared_ptr<object>(new identifier("TODO"));
}

void interpret(std::vector<std::unique_ptr<object>> &trees, std::ostream &os) {
	env_type env;
for (const auto &tree : trees) {
		os << *interpret_next_expression(tree, env) << std::endl;
	}
}
