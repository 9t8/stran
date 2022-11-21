#include "datum.h"

#include <sstream>
#include <cassert>

std::shared_ptr<const datum> procedure::call(
	env_type &env, std::vector<std::unique_ptr<const datum>> &args
) const {
	assert(args.size() == params.size() && "incorrect number of arguments");

	env_type new_env(env);
	for (size_t i(0); i < params.size(); ++i) {
		new_env[params[i]] = std::move(args[i]);
	}
	return body->eval(new_env);
}

list::operator std::string() const {
	std::ostringstream oss;
	oss << "[";
	if (!elements.empty()) {
		oss << *elements.front();
		for (size_t i(1); i != elements.size(); ++i) {
			oss << " " << *elements[i];
		}
	}
	oss << "]";
	return oss.str();
}
