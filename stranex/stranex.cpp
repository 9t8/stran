// r3rs interpreter with boost for numeric tower
// lex code to token list
// parse to pre-cons tree
// consify to post-cons tree
// interprets to output

#include "lex.h"
#include "parse.h"
//#include "interpret.h"

#include <iostream>

int main(int argc, const char *argv[]) {
	std::deque<std::unique_ptr<token>> tokens(lex(std::cin));

	std::cerr << "===-- tokens --===\n";
for (const auto &token : tokens) {
		std::cerr << *token << "\n";
	}

	std::vector<std::unique_ptr<token>> trees(parse(tokens));

	std::cerr << "===-- pre-cons trees --===\n";
for (const auto &tree : trees) {
		std::cerr << *tree << "\n";
	}
}
