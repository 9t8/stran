#include "lex.h"

#include <cassert>
#include <iostream>
#include <sstream>

std::unique_ptr<token> read_next(std::istream &is) {
	auto next_char(is.peek());

	if (next_char == '(') {
		is.get();
		return std::unique_ptr<beginl>(new beginl);
	}

	if (next_char == ')') {
		is.get();
		return std::unique_ptr<endl>(new endl);
	}

	if (next_char == '+' || next_char == '-'
		|| next_char == '.' || std::isdigit(next_char)) {
		double val;
		is >> val;
		return std::unique_ptr<decimal>(new decimal(val));
	}

	if (std::isgraph(next_char)) {
		std::ostringstream name;

		for (;; next_char = is.peek()) {
			if (next_char == '(') {
				assert(0 && "illegal character in identifier: (");
			}

			if (next_char == ')' || std::isspace(next_char)) {
				std::string name_str(name.str());
				assert(!name_str.empty() && "name is empty somehow");
				return std::unique_ptr<identifier>(new identifier(name_str));
			}

			name << static_cast<char>(is.get());
		}
	}

	std::cerr << "ERROR - unexpected character type: '" << static_cast<char>(next_char)
	<< "' (character code " << next_char << ")\n";

	assert(0 && "unexpected character type - see std::cerr");
	throw; // suppress warning
}

std::deque<std::unique_ptr<token>> lex(std::istream &is) {
	std::deque<std::unique_ptr<token>> tokens;
	is >> std::ws;
	while (!is.eof()) {
		tokens.push_back(read_next(is));
		is >> std::ws;
	}
	return tokens;
}
