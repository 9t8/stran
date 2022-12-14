#include "datum.h"

#include <sstream>

#include <iostream>

p_datum procedure::call(const p_datum &args, environment &env) const {
	environment new_env(create_new_env(args, env));
	return body->eval(new_env);
}

environment procedure::create_new_env(const p_datum &args, environment &env) const {
	environment new_env(env);

	const pair *curr_arg(dynamic_cast<const pair *>(args.get()));

	if (formals.empty()) {
		assert(curr_arg == nullptr && "too many arguments");

		return new_env;
	}

	auto next_arg([&] {
		assert(curr_arg != nullptr && "malformed argument list (not enough arguments?)");

		p_datum result(curr_arg->car->eval(env));
		curr_arg = dynamic_cast<const pair *>(curr_arg->cdr.get());
		return result;
	});

	for (size_t i(0); i < formals.size() - 1; ++i) {
		new_env[formals[i]] = next_arg();
	}

	if (!variadic) {
		new_env[formals.back()] = next_arg();
		assert(curr_arg == nullptr && "too many arguments");

		return new_env;
	}

	if (curr_arg == nullptr) {
		new_env[formals.back()] = std::make_shared<empty_list>();
		return new_env;
	}

	// sussy stuff here: check for leaks
	std::shared_ptr<pair> tail(std::make_shared<pair>(next_arg()));
	std::cerr << "\t" << tail.use_count() << "\n";
	while (curr_arg != nullptr) {
		std::shared_ptr<pair> new_tail(std::make_shared<pair>(next_arg()));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	new_env[formals.back()] = tail;
	return new_env;
}

bool pair::stringify_into_lists(true);

// write iteratively, use ostringstream
std::string stringify_elements(const pair &p) {
	return static_cast<std::string>(*p.car) + " . " + static_cast<std::string>(*p.cdr);
}

pair::operator std::string() const {
	return "(" + stringify_elements(*this) + ")";
}
