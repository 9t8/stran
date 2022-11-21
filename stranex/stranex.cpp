// r3rs interpreter with boost for numeric tower

// virtual inheritance?

#include "lex.h"
#include "parse.h"
#include "eval.h"

#include <iostream>

int main(int argc, const char **argv) {
	std::deque<std::unique_ptr<token>> tokens(lex(std::cin));

	std::cout << "===-- tokens --===\n" << std::flush;
	for (size_t i(0); i != tokens.size(); ++i) {
		std::cout << *tokens[i] << std::endl;
	}

	std::vector<std::shared_ptr<const datum>> trees(parse(tokens));

	std::cout << "===-- trees --===\n" << std::flush;
	for (size_t i(0); i != trees.size(); ++i) {
		std::cout << *trees[i] << std::endl;
	}

	std::cout << "===-- output --===\n" << std::flush;
	eval(trees, std::cout);
}
