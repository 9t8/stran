#include "lex.h"

#include <cassert>
#include <iostream>

std::unique_ptr<token> lex_next_token(std::istream &in) {
	auto next_char(in.peek());

	if (next_char == '(') {
		in.get();
		return std::unique_ptr<beginl>(new beginl);
	}

	if (next_char == ')') {
		in.get();
		return std::unique_ptr<endl>(new endl);
	}

	if (std::isalpha(next_char)) {
		std::ostringstream name;

		for (;; next_char = in.peek()) {
			if (next_char == '(') {
				assert(0 && "illegal character in identifier: (");
			}

			if (next_char == ')' || std::isspace(next_char)) {
				return std::unique_ptr<identifier>(new identifier(name.str()));
			}

			name << static_cast<char>(in.get());
		}
	}

	if (next_char == '+' || next_char == '-'
		|| next_char == '.' || std::isdigit(next_char)) {
		double val;
		in >> val;
		return std::unique_ptr<decimal>(new decimal(val));
	}

	std::cerr << "ERROR - unexpected character type: '" << static_cast<char>(next_char)
	<< "' (character number " << next_char << ")\n";

	assert(0 && "unexpected character type - see std::cerr");
	throw; // suppress warning
}

std::deque<std::unique_ptr<token>> lex(std::istream &in) {
	std::deque<std::unique_ptr<token>> tokens;
	in >> std::ws;
	while (!in.eof()) {
		tokens.push_back(lex_next_token(in));
		in >> std::ws;
	}
	return tokens;
}
