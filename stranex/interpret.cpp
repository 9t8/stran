#include "interpret.h"

std::shared_ptr<token> interpret_next_expression(const std::unique_ptr<token> &p_ast,
		env_type &env) {
	/*if (typeid(*p_ast) == typeid(list)) {
		return "LIST\n";
	}*/
	return std::shared_ptr<token>(new identifier("TODO"));
}
