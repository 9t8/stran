#include "eval.h"
#include "lex.h"

#include <iostream>

std::vector<p_datum> parse(token_list &tokens) {
	std::vector<p_datum> syntax_tree;
	while (!tokens.empty()) {
		syntax_tree.push_back(tokens.front()->parse(tokens));
	}
	return syntax_tree;
}

int main(int, const char **) {
	filtered_input fi(std::cin);
	token_list tokens(lex(fi));

	std::cout << "===-- tokens --===" << std::endl;
	if (!tokens.empty()) {
		for (size_t i(0); i < tokens.size() - 1; ++i) {
			std::cout << *tokens[i] << " " << std::flush;
		}
		std::cout << *tokens.back() << std::endl;
	}

	std::vector<p_datum> syntax_tree(parse(tokens));

	std::cout << "\n===-- syntax tree --===" << std::endl;
	for (size_t i(0); i < syntax_tree.size(); ++i) {
		std::cout << "  " << *syntax_tree[i] << std::endl;
	}

	std::cout << "\n===-- output --===" << std::endl;
	eval(syntax_tree, std::cout);
}
