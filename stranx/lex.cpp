#include "lex.h"

void lex_token(token_list &tokens, std::string &s) {
	assert(!s.empty() && "attempted to tokenize an empty string");

	if (s == ".") {
		tokens.push_back(std::make_unique<dot>());
		return;
	}

	if (isdigit(s[0]) || s[0] == '+' || s[0] == '-' || s[0] == '.') {
		size_t idx(0);
		double val(stod(s, &idx));
		assert(idx == s.size() && "invalid character while parsing decimal");
		tokens.push_back(std::make_unique<decimal>(val));
		return;
	}

	tokens.push_back(std::make_unique<identifier>(s));
}

token_list lex(filtered_input &fi) {
	token_list tokens;

	std::string curr_word;
	for (;;) {
		int curr_char(fi.get());

		if (!isspace(curr_char) && curr_char != EOF && curr_char != '(' && curr_char != ')' &&
			curr_char != '"' && curr_char != ';') {
			curr_word += curr_char;
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
				tokens.push_back(std::make_unique<begin_list>());
				continue;

			case ')':
				tokens.push_back(std::make_unique<end_list>());
				continue;

			case ';':
				while (curr_char != '\n' && curr_char != EOF) {
					curr_char = fi.get();
				}
				continue;
		}

		assert(0 && "character not yet supported");
	}
}
