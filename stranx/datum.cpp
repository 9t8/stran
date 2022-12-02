#include "datum.h"

#include <sstream>

p_datum procedure::internal_call(environment &env,
		const std::vector<p_datum> &args) const {
	environment new_env(env);
	for (size_t i(0); i != formals.size(); ++i) {
		new_env[formals[i]] = args[i]->eval(env);
	}
	return body->eval(new_env);
}

/*list::operator std::string() const {
	std::ostringstream oss;
	oss << "[";
	if (!elements.empty()) {
		for (size_t i(0); i != elements.size() - 1; ++i) {
			oss << *elements[i] << " ";
		}
		oss << *elements.back();
	}
	oss << "]";
	return oss.str();
}*/

bool pair::stringify_into_lists(true);

std::string stringify_elements(const pair &p) {
	return static_cast<std::string>(*p.car) + " . " + static_cast<std::string>(*p.cdr);
}

pair::operator std::string() const {
	return "(" + stringify_elements(*this) + ")";
}
