#include "object.h"

#include <sstream>

const std::type_info &get_next_token_type(std::deque<std::unique_ptr<token>> &tokens) {
	assert(!tokens.empty() && "expected a token but none found (too many opening parens?)");

	token &next_token(*tokens.front()); // suppress warning
	return typeid(next_token);
}

std::unique_ptr<object> beginl::parse(std::deque<std::unique_ptr<token>> &tokens) const {
	std::unique_ptr<list> p_list(new list);
	tokens.pop_front();
	while (get_next_token_type(tokens) != typeid(endl)) {
		p_list->elements.push_back(tokens.front()->parse(tokens));
	}
	tokens.pop_front();
	return p_list;
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

bool pair::stringify_into_lists(true);

std::string stringify_elements(const pair &p) {
	return static_cast<std::string>(*p.car) + " . " + static_cast<std::string>(*p.cdr);
}

pair::operator std::string() const {
	return "(" + stringify_elements(*this) + ")";
}
