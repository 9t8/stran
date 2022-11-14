#include "interpret.h"

void interpret(std::vector<std::unique_ptr<object>> &trees, std::ostream &os) {
	env_type env;
	for (size_t i(0); i < trees.size(); ++i) {
		/*os << trees[i]->interpret(env) << std::endl;*/
	}
}
