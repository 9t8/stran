#include "token.h"

#include <iostream>

p_datum begin_list::parse(token_list &tokens) const {
	tokens.pop_front();

	auto get_next_token_type([&]() -> const std::type_info & {
		assert(!tokens.empty() && "expected a token but none found (too many opening parens?)");

		token &next_token(*tokens.front());
		return typeid(next_token);
	});

	if (get_next_token_type() == typeid(end_list)) {
		tokens.pop_front();
		return std::make_shared<empty_list>();
	}

	std::shared_ptr<pair> p(std::make_shared<pair>(tokens.front()->parse(tokens)));
	p_datum start(p);
	while (get_next_token_type() != typeid(end_list)) {
		if (get_next_token_type() == typeid(dot)) {
			tokens.pop_front();
			p->cdr = tokens.front()->parse(tokens);
			assert(get_next_token_type() == typeid(end_list) &&
				   "malformed improper list (misplaced dot token)");
			break;
		}
		std::shared_ptr<pair> p_new(std::make_shared<pair>(tokens.front()->parse(tokens)));
		p->cdr = p_new;
		p = p_new;
	}
	tokens.pop_front();

	return start;
}

p_datum identifier::eval(const p_env &env) {
	p_env::element_type::iterator it(env->find(name));
	if (it == env->end()) {
		std::cerr << "ERROR - undefined identifier: " << name << "\n";
		throw;
	}

	return it->second;
}
