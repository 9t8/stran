// r3rs interpreter with boost for numeric tower
// lex code to token list
// parse to syntax tree
// interprets to output and environment

// virtual inheritance?

// ways to implement empty list
// null pointer
// subclass

#include "lex.h"
#include "parse.h"

#include <iostream>

int main(int argc, const char **argv) {
	std::deque<std::unique_ptr<token>> tokens(lex(std::cin));

	std::cerr << "===-- tokens --===\n";
	for (size_t i(0); i != tokens.size(); ++i) {
		std::cerr << *tokens[i] << "\n";
	}

	std::vector<std::unique_ptr<datum>> trees(parse(tokens));

	std::cerr << "===-- trees --===\n";
	for (size_t i(0); i != trees.size(); ++i) {
		std::cerr << *trees[i] << "\n";
	}
}
