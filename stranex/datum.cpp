#include "datum.h"

#include <sstream>

p_datum procedure::internal_call(
	environment &env, const syntax_tree &args
) const {
	environment new_env(env);
	for (size_t i(0); i < formals.size(); ++i) {
		new_env[formals[i]] = args[i]->eval(env);
	}
	return body->eval(new_env);
}

list::operator std::string() const {
	std::ostringstream oss;
	oss << "[";
	if (!elements.empty()) {
		oss << *elements.front();
		for (size_t i(1); i != elements.size(); ++i) {
			oss << " " << *elements[i];
		}
	}
	oss << "]";
	return oss.str();
}
