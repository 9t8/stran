#include "datum.h"

#include <iostream>

p_datum procedure::call(const p_datum &args, const_p_env &) const {
	const_p_env new_env(create_new_env(args));

	p_pair exprs(body);
	p_datum result(next(exprs)->eval(new_env));
	while (exprs) {
		result = next(exprs)->eval(new_env);
	}
	return result;
}

const_p_env procedure::create_new_env(const p_datum &args) const {
	const_p_env new_env(std::make_shared<const_p_env::element_type>(*env));

	p_pair curr_arg(std::dynamic_pointer_cast<pair>(args));

	if (formals.empty()) {
		assert(!curr_arg && "too many arguments");

		return new_env;
	}

	for (size_t i(0); i < formals.size() - 1; ++i) {
		(*new_env)[formals[i]] = next(curr_arg)->eval(env);
	}

	if (!variadic) {
		(*new_env)[formals.back()] = next(curr_arg)->eval(env);
		assert(!curr_arg && "too many arguments");

		return new_env;
	}

	if (!curr_arg) {
		(*new_env)[formals.back()] = std::make_shared<empty_list>();
		return new_env;
	}

	p_pair tail(std::make_shared<pair>(next(curr_arg)->eval(env)));
	(*new_env)[formals.back()] = tail;
	while (curr_arg) {
		p_pair new_tail(std::make_shared<pair>(next(curr_arg)->eval(env)));
		tail->cdr = new_tail;
		tail = new_tail;
	}
	return new_env;
}

bool pair::stringify_into_lists(true);

pair::operator std::string() const {
	if (!stringify_into_lists) {
		return "(" + static_cast<std::string>(*car) + " . " + static_cast<std::string>(*cdr)
			   + ")";
	}

	std::ostringstream oss;
	oss << "(" << *car;

	p_datum p_last(cdr);
	for (p_pair curr_pair(std::dynamic_pointer_cast<pair>(cdr)); curr_pair;) {
		p_last = curr_pair->cdr;
		oss << " " << *next(curr_pair);
	}

	const datum &last(*p_last);
	if (typeid(last) != typeid(empty_list)) {
		oss << " . " << last;
	}
	oss << ")";

	return oss.str();
}

const p_datum &next(p_pair &exprs) {
	assert(exprs && "invalid expression list (not enough arguments?)");

	const p_datum &result(exprs->car);
	exprs = std::dynamic_pointer_cast<pair>(exprs->cdr);
	return result;
}

p_datum identifier::eval(const_p_env &env) {
	const_p_env::element_type::iterator it(env->find(name));
	if (it == env->end()) {
		std::cerr << "ERROR - undefined identifier: " << name << "\n";
		throw;
	}

	return it->second;
}
