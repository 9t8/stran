#include "token.h"

const std::type_info &get_next_token_type(token_list &tokens) {
	assert(!tokens.empty() && "expected a token but none found (too many opening parens?)");

	token &next_token(*tokens.front());
	return typeid(next_token);
}

p_datum begin_list::parse(token_list &tokens) const {
	std::shared_ptr<list> p(new list);
	tokens.pop_front();
	while (get_next_token_type(tokens) != typeid(end_list)) {
		p->elements.push_back(tokens.front()->parse(tokens));
	}
	tokens.pop_front();
	return p;
}
