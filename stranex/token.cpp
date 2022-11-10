#include "token.h"

#include <sstream>

list::operator std::string() const {
	std::ostringstream oss;
	oss << "[";
	if (!elements.empty()) {
		oss << *elements.front();
		for (int i(1); i < elements.size(); ++i) {
			oss << " " << *elements[i];
		}
	}
	oss << "]";
	return oss.str();
}

bool pair::stringify_into_lists(true);

std::string stringify_elements(const pair &p) {
	return static_cast<std::string>(*p.car) + " . " + static_cast<std::string>(*p.cdr);
}

pair::operator std::string() const {
	return "(" + stringify_elements(*this) + ")";
}
