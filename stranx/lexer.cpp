#include "lexer.h"

#include <cassert>

void lex_token(token_list &tokens, std::string &s) {
	assert(!s.empty() && "attempted to tokenize an empty string");

	if (s == ".") {
		tokens.push_back(std::make_shared<dot>());
		return;
	}

	if (isdigit(s[0]) || s[0] == '+' || s[0] == '-' || s[0] == '.') {
		size_t idx(0);
		double val(stod(s, &idx));
		assert(idx == s.size() && "invalid character while parsing decimal");
		tokens.push_back(std::make_shared<decimal>(val));
		return;
	}

	tokens.push_back(std::make_shared<identifier>(s));
}

token_list lexer::lex() {
	token_list tokens;

	std::string curr_word;
	for (;;) {
		int curr_char(get());

		if (!isspace(curr_char) && curr_char != EOF && curr_char != '(' && curr_char != ')' &&
			curr_char != '"' && curr_char != ';') {
			curr_word += static_cast<char>(curr_char);
			continue;
		}

		if (!curr_word.empty()) {
			lex_token(tokens, curr_word);
			curr_word.clear();
		}

		if (isspace(curr_char)) {
			continue;
		}

		switch (curr_char) {
			case EOF:
				return tokens;

			case '(':
				tokens.push_back(std::make_shared<begin_list>());
				continue;

			case ')':
				tokens.push_back(std::make_shared<end_list>());
				continue;

			case ';':
				while (curr_char != '\n' && curr_char != EOF) {
					curr_char = get();
				}
				continue;
		}

		assert(0 && "character not yet supported");
	}
}

int lexer::get() {
	int c(is.get());

	switch (c) {
		case '\n':
			col = 0;
			++row;
		case EOF:
			return c;
	}

	if (std::isprint(c) || c == '\t') {
		++col;
		return c;
	}

	std::cerr << "ERROR - unexpected character: character code " << c << "\n";
	throw;
}
