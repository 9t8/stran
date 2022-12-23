#include "eval.h"
#include "lexer.h"

#include <iostream>

p_datum parse_next(const token_list &tokens, size_t &idx) {
	static auto get_next_token_type([&]() -> const std::type_info & {
		assert(idx < tokens.size() &&
		"expected a token but none found (too many opening parens?)");

		object &next_token(*tokens[idx]);
		return typeid(next_token);
	});

	if (get_next_token_type() == typeid(begin_list)) {
		++idx;
		if (get_next_token_type() == typeid(end_list)) {
			++idx;
			return std::make_shared<empty_list>();
		}

		std::shared_ptr<pair> p(std::make_shared<pair>(parse_next(tokens, idx)));
		p_datum start(p);
		while (get_next_token_type() != typeid(end_list)) {
			if (get_next_token_type() == typeid(dot)) {
				p->cdr = parse_next(tokens, ++idx);
				assert(get_next_token_type() == typeid(end_list) &&
					   "malformed improper list (misplaced dot token)");
				break;
			}
			std::shared_ptr<pair> p_new(std::make_shared<pair>(parse_next(tokens, idx)));
			p->cdr = p_new;
			p = p_new;
		}
		++idx;

		return start;
	}

	p_datum p(std::dynamic_pointer_cast<datum>(tokens.at(idx++)));
	assert(p && "malformed token list: tried to parse an unparsable token");

	return p;
}

std::vector<p_datum> parse(token_list &tokens) {
	std::vector<p_datum> tree;
	size_t idx(0);
	while (idx < tokens.size()) {
		tree.push_back(parse_next(tokens, idx));
	}
	return tree;
}

int main(int, const char *[]) {
	token_list tokens(lexer(std::cin).lex());

	std::cout << "===-- tokens --===" << std::endl;
	if (!tokens.empty()) {
		for (size_t i(0); i < tokens.size() - 1; ++i) {
			std::cout << *tokens[i] << " " << std::flush;
		}
		std::cout << *tokens.back() << std::endl;
	}

	std::vector<p_datum> tree(parse(tokens));

	std::cout << "\n===-- tree --===" << std::endl;
	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << "  " << *tree[i] << std::endl;
	}

	std::cout << "\n===-- output --===" << std::endl;
	eval(tree, std::cout);
}
