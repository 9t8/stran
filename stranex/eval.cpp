#include "eval.h"

#include <ostream>

void eval(std::vector<std::shared_ptr<const datum>> &trees, std::ostream &os) {
	env_type env;
	for (size_t i(0); i < trees.size(); ++i) {
		os << trees[i]->eval(env) << std::endl;
	}
}
