#include "datum.h"

#include <iostream>
#include <sstream>

const p_datum &context::find(const std::string &name) {
	decltype(table)::iterator it(table.find(name));
	if (it != table.end()) {
		return it->second;
	}
	if (!parent) {
		std::cerr << "ERROR - undefined identifier: " << name << "\n";
		throw;
	}
	return parent->find(name);
}

bool pair::stringify_into_lists(true);

pair::operator std::string() const {
	if (!stringify_into_lists) {
		return "(" + static_cast<std::string>(*car) + " . " + static_cast<std::string>(*cdr)
			   + ")";
	}

	std::ostringstream oss;
	oss << "(" << *car;

	p_datum p_last(cdr);
	for (p_pair curr_pair(std::dynamic_pointer_cast<pair>(cdr)); curr_pair;) {
		p_last = curr_pair->cdr;
		oss << " " << *next(curr_pair);
	}

	const datum &last(*p_last);
	if (typeid(last) != typeid(emptyl)) {
		oss << " . " << last;
	}
	oss << ")";

	return oss.str();
}

const p_datum &next(p_pair &exprs) {
	assert(exprs && "invalid expression list (not enough arguments?)");

	const p_datum &result(exprs->car);
	exprs = std::dynamic_pointer_cast<pair>(exprs->cdr);
	return result;
}
