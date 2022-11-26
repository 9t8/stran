#include "pair.h"

bool pair::stringify_into_lists(true);

std::string stringify_elements(const pair &p) {
	return static_cast<std::string>(*p.car) + " . " + static_cast<std::string>(*p.cdr);
}

pair::operator std::string() const {
	return "(" + stringify_elements(*this) + ")";
}
