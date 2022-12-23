#include "token.h"

p_datum begin_list::parse(const token_list &tokens, size_t &idx) const {
	assert(tokens.at(idx).get() == this && "first token in tokens is not current token");

	++idx;

	auto get_next_token_type([&]() -> const std::type_info & {
		assert(idx < tokens.size() &&
		"expected a token but none found (too many opening parens?)");

		token &next_token(*tokens[idx]);
		return typeid(next_token);
	});

	if (get_next_token_type() == typeid(end_list)) {
		++idx;
		return std::make_shared<empty_list>();
	}

	std::shared_ptr<pair> p(std::make_shared<pair>(tokens.at(idx)->parse(tokens, idx)));
	p_datum start(p);
	while (get_next_token_type() != typeid(end_list)) {
		if (get_next_token_type() == typeid(dot)) {
			++idx;
			p->cdr = tokens.at(idx)->parse(tokens, idx);
			assert(get_next_token_type() == typeid(end_list) &&
				   "malformed improper list (misplaced dot token)");
			break;
		}
		std::shared_ptr<pair> p_new(std::make_shared<pair>(tokens.at(idx)->parse(tokens, idx)));
		p->cdr = p_new;
		p = p_new;
	}
	++idx;

	return start;
}
