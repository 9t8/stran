#include "func.h"

#include <sstream>

using namespace stranx;

bool pair::stringify_into_lists(true);

pair::operator std::string() const {
	if (!stringify_into_lists) {
		return "(" + static_cast<std::string>(*car) + " . " + static_cast<std::string>(*cdr)
			   + ")";
	}

	std::ostringstream oss;
	oss << "(" << *car;

	sp<datum> p_last(cdr);
	for (sp<pair> curr_pair(std::dynamic_pointer_cast<pair>(cdr)); curr_pair;) {
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

const sp<datum> &stranx::next(sp<pair> &exprs) {
	assert(exprs && "invalid expression list (not enough arguments?)");

	const sp<datum> &result(exprs->car);
	exprs = std::dynamic_pointer_cast<pair>(exprs->cdr);
	return result;
}
