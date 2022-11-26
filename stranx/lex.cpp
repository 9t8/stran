#include "lex.h"

#include <iostream>
#include <sstream>

std::unique_ptr<token> read_next(std::istream &is) {
	auto next_char(is.peek());

	if (next_char == '(') {
		is.get();
		return std::unique_ptr<token>(new begin_list);
	}

	if (next_char == ')') {
		is.get();
		return std::unique_ptr<token>(new end_list);
	}

	if (next_char == '+' || next_char == '-'
		|| next_char == '.' || std::isdigit(next_char)) {
		double val;
		is >> val;
		return std::unique_ptr<token>(new decimal(val));
	}

	if (std::isgraph(next_char)) {
		std::ostringstream name;

		for (;; next_char = is.peek()) {
			if (next_char == '(') {
				assert(0 && "illegal character in identifier: (");
			}

			if (next_char == ')' || std::isspace(next_char)) {
				std::string name_str(name.str());
				assert(!name_str.empty() && "name is (somehow) empty");

				return std::unique_ptr<token>(new identifier(name_str));
			}

			name << static_cast<char>(is.get());
		}
	}

	std::cerr << "ERROR - unexpected character type: '" << static_cast<char>(next_char)
	<< "' (character code " << next_char << ")\n";

	assert(0 && "unexpected character type - see stderr");
	throw; // suppress warning
}

token_list lex(std::istream &is) {
	token_list tokens;
	// todo: switch to lexing line-by-line and even char-by-char
	is >> std::ws;
	while (!is.eof()) {
		tokens.push_back(read_next(is));
		is >> std::ws;
	}
	return tokens;
}
