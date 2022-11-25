// r3rs interpreter

// todo
// add comment support
// add dot token
// change vectors to pair chains (dot can be a special token)?
// pairs
// actual input handling
// use boost for full numerical tower

#include "lex.h"
#include "parse.h"
#include "eval.h"

#include <iostream>

int main(int argc, const char **argv) {
	token_list tokens(lex(std::cin));

	if (tokens.empty()) {
		std::cout << "no tokens - exiting\n";
		return 0;
	}
	std::cout << "===-- tokens --===" << std::endl;
	for (size_t i(0); i != tokens.size() - 1; ++i) {
		std::cout << *tokens[i] << " " << std::flush;
	}
	std::cout << *tokens.back() << std::endl;

	std::vector<p_datum> syntax_tree(parse(tokens));

	std::cout << "===-- syntax tree --===" << std::endl;
	for (size_t i(0); i != syntax_tree.size(); ++i) {
		std::cout << *syntax_tree[i] << std::endl;
	}

	std::cout << "===-- output --===" << std::endl;
	eval(syntax_tree, std::cout);
}
