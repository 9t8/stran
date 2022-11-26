#include "parse.h"

std::vector<p_datum> parse(token_list &tokens) {
	std::vector<p_datum> syntax_tree;
	while (!tokens.empty()) {
		syntax_tree.push_back(tokens.front()->parse(tokens));
	}
	return syntax_tree;
}
