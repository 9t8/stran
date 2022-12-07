#include "datum.h"

#include <sstream>

p_datum procedure::call(environment &env, const p_datum &args) const {
	environment new_env(env);

	// fixme: is this safe?
	std::shared_ptr<const pair> curr_arg(dynamic_cast<const pair *>(args.get()));
	for (size_t i(0); i < formals.size() - 1; ++i) {
		assert(curr_arg != nullptr && "malformed argument list");

		new_env[formals[i]] = curr_arg->car->eval(env);
		curr_arg.reset(dynamic_cast<const pair *>(curr_arg->cdr.get()));
	}
	if (variadic) {
		std::shared_ptr<pair> pack(new pair);

		new_env[formals[i]] = pack;
	} else {
		new_env[formals[i]] = curr_arg->car->eval(env);
		assert(typeid(*curr_arg->car) == typeid(empty_list));
	}

	return body->eval(new_env);
}

bool pair::stringify_into_lists(true);

// write iteratively, use ostringstream
std::string stringify_elements(const pair &p) {
	return static_cast<std::string>(*p.car) + " . " + static_cast<std::string>(*p.cdr);
}

pair::operator std::string() const {
	return "(" + stringify_elements(*this) + ")";
}
