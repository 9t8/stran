#include "parse.h"

std::vector<std::shared_ptr<const datum>> parse(std::deque<std::unique_ptr<token>> &tokens) {
	std::vector<std::shared_ptr<const datum>> trees;
	while (!tokens.empty()) {
		trees.push_back(tokens.front()->parse(tokens));
	}
	return trees;
}
