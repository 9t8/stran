#include "eval.h"

#include <ostream>

void eval(syntax_tree &tree, std::ostream &os) {
	env_type env;
	for (size_t i(0); i < tree.size(); ++i) {
		os << *tree[i]->eval(env) << std::endl;
	}
}
