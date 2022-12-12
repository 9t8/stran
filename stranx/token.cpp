#include "token.h"

const std::type_info &get_next_token_type(token_list &tokens) {
	assert(!tokens.empty() && "expected a token but none found (too many opening parens?)");

	token &next_token(*tokens.front());
	return typeid(next_token);
}

p_datum begin_list::parse(token_list &tokens) const {
	tokens.pop_front();

	if (get_next_token_type(tokens) == typeid(end_list)) {
		tokens.pop_front();
		return std::make_shared<empty_list>();
	}

	std::shared_ptr<pair> start(std::make_shared<pair>());
	start->car = tokens.front()->parse(tokens);

	std::shared_ptr<pair> p(start);
	while (get_next_token_type(tokens) != typeid(end_list)) {
		std::shared_ptr<pair> p_new(std::make_shared<pair>());
		p_new->car = tokens.front()->parse(tokens);
		p->cdr = p_new;
		p = p_new;
	}
	tokens.pop_front();

	return start;
}
