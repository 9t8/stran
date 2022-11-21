#include "datum.h"

#include <sstream>

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

// token stuff

const std::type_info &get_next_token_type(token_list &tokens) {
	assert(!tokens.empty() && "expected a token but none found (too many opening parens?)");

	token &next_token(*tokens.front()); // suppress warning
	return typeid(next_token);
}

std::shared_ptr<const datum> begin_list::parse(token_list &tokens) const {
	std::shared_ptr<list> p(new list);
	tokens.pop_front();
	while (get_next_token_type(tokens) != typeid(end_list)) {
		p->elements.push_back(tokens.front()->parse(tokens));
	}
	tokens.pop_front();
	return p;
}