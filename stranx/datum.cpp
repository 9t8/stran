#include "datum.h"

#include <sstream>

#include <iostream>

p_datum procedure::call(const p_datum &args, environment &env) const {
	environment new_env(create_new_env(args, env));
	return body->eval(new_env);
}

p_datum eval_arg(const pair *&curr_arg, environment &env) {
	p_datum result(curr_arg->car->eval(env));
	curr_arg = dynamic_cast<const pair *>(curr_arg->cdr.get());
	return result;
}

environment procedure::create_new_env(const p_datum &args, environment &env) const {
	// proofread this method
	
	environment new_env(env);

	const pair *curr_arg(dynamic_cast<const pair *>(args.get()));

	for (size_t i(0); i < formals.size() - 1; ++i) {
		assert(curr_arg != nullptr && "malformed argument list (not enough arguments?)");

		new_env[formals[i]] = eval_arg(curr_arg, env);
	}

	if (!variadic) {
		if (!formals.empty()) {
			new_env[formals.back()] = eval_arg(curr_arg, env);
		}
		assert(curr_arg == nullptr && "too many arguments");

		return new_env;
	}

	if (curr_arg == nullptr) {
		new_env[formals.back()] = std::make_shared<empty_list>();
		return new_env;
	}

	std::shared_ptr<pair> tail(std::make_shared<pair>());
	p_datum pack(tail);
	std::cerr << "\t" << tail.use_count() << "\n";
	tail->car = eval_arg(curr_arg, env);
	while (curr_arg != nullptr) {
		std::shared_ptr<pair> new_tail(std::make_shared<pair>());
		new_tail->car = eval_arg(curr_arg, env);
		tail->cdr = new_tail;
		tail = new_tail;
		new_tail = std::make_shared<pair>();
	}
	new_env[formals.back()] = pack;
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
