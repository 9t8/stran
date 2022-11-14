#include "parse.h"

std::vector<std::unique_ptr<datum>> parse(std::deque<std::unique_ptr<token>> &tokens) {
	std::vector<std::unique_ptr<datum>> trees;
	while (!tokens.empty()) {
		trees.push_back(tokens.front()->parse(tokens));
	}
	return trees;
}
