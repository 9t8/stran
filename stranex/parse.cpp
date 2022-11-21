#include "parse.h"

syntax_tree parse(token_list &tokens) {
	syntax_tree tree;
	while (!tokens.empty()) {
		tree.push_back(tokens.front()->parse(tokens));
	}
	return tree;
}
