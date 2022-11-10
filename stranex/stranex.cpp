// r3rs interpreter with boost for numeric tower
// lex code to token list
// parse to syntax tree
// interprets to output and environment

#include "lex.h"
#include "parse.h"

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
