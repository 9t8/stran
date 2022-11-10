#include "parse.h"

#include <cassert>

const std::type_info &get_next_token_type(std::deque<std::unique_ptr<token>> &tokens) {
	assert(!tokens.empty() && "expected a token but none found (too many opening parens?)");

	return typeid(*tokens.front());
}

std::unique_ptr<token> parse_next_expression(std::deque<std::unique_ptr<token>> &tokens) {
	const std::type_info &next_token_type(get_next_token_type(tokens));

	if (next_token_type == typeid(endl)) {
		assert(0 && "unexpected end of list token");
	}

	if (next_token_type == typeid(beginl)) {
		std::unique_ptr<list> p_list(new list);
		tokens.pop_front();
		while (get_next_token_type(tokens) != typeid(endl)) {
			p_list->elements.push_back(parse_next_expression(tokens));
		}
		tokens.pop_front();
		return p_list;
	}

	std::unique_ptr<token> p_token(std::move(tokens.front()));
	tokens.pop_front();
	return p_token;
}

std::vector<std::unique_ptr<token>> parse(std::deque<std::unique_ptr<token>> &tokens) {
	std::vector<std::unique_ptr<token>> trees;
	while (!tokens.empty()) {
		trees.push_back(parse_next_expression(tokens));
	}
	return trees;
}
