#include "eval.h"
#include "lex.h"
#include "parse.h"

#include <iostream>

int main(int argc, const char **argv) {
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
