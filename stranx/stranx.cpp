#include "eval.h"
#include "lexer.h"

#include <iostream>

std::vector<p_datum> parse(token_list &tokens) {
	std::vector<p_datum> tree;
	size_t idx(0);
	while (idx < tokens.size()) {
		tree.push_back(tokens[idx]->parse(tokens, idx));
	}
	return tree;
}

int main(int, const char **) {
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
