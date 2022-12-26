#include "lexer.h"
#include "closure.h"

#include <iostream>

p_datum parse_next(const token_list &tokens, size_t &idx) {
	const auto get_next_token_type([&]() -> const std::type_info & {
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

		p_pair p(std::make_shared<pair>(parse_next(tokens, idx)));
		p_datum start(p);
		while (get_next_token_type() != typeid(end_list)) {
			if (get_next_token_type() == typeid(dot)) {
				p->cdr = parse_next(tokens, ++idx);
				assert(get_next_token_type() == typeid(end_list) &&
					   "malformed improper list (misplaced dot token)");
				break;
			}
			p_pair p_new(std::make_shared<pair>(parse_next(tokens, idx)));
			p->cdr = p_new;
			p = p_new;
		}
		++idx;

		return start;
	}

	p_datum p(std::dynamic_pointer_cast<datum>(tokens.at(idx++)));
	assert(p && "malformed token list (tried to parse an unparsable token)");

	return p;
}

int main(int, const char *[]) {
	token_list tokens(lexer(std::cin).lex());

	std::cout << "===-- tokens --===\n";
	if (!tokens.empty()) {
		for (size_t i(0); i < tokens.size() - 1; ++i) {
			std::cout << *tokens[i] << " ";
		}
		std::cout << *tokens.back() << "\n";
	}

	std::vector<p_datum> tree;
	for (size_t i(0); i < tokens.size();) {
		tree.push_back(parse_next(tokens, i));
	}

	std::cout << "\n===-- tree --===\n";
	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << "  " << *tree[i] << "\n";
	}
	std::cout << "\n===-- output --===" << std::endl;

	const p_env env(std::make_shared<environment>(nullptr));
	env->define("lambda", std::make_shared<lambda>());
	env->define("define", std::make_shared<define>());

	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << *tree[i]->eval(env) << std::endl;
	}
}
