#include "lex.h"

p_token lex_token(std::string &s) {
	assert(!s.empty() && "attempted to tokenize an empty string");

	if (isdigit(s[0]) || s[0] == '+' || s[0] == '-' || s[0] == '.') {
		size_t idx(0);
		double val(stod(s, &idx));
		assert(idx == s.size() && "invalid character in decimal");
		return p_token(new decimal(val));
	}

	return p_token(new identifier(s));
}

token_list lex(filtered_input &fi) {
	token_list tokens;

	std::string curr_word;
	for (;;) {
		int curr_char(fi.get());

		if (!isspace(curr_char) && curr_char != EOF && curr_char != '(' && curr_char != ')'
			&& curr_char != '"' && curr_char != ';') {
			curr_word += curr_char;
			continue;
		}

		if (!curr_word.empty()) {
			tokens.push_back(lex_token(curr_word));
			curr_word.clear();
		}

		if (isspace(curr_char)) {
			continue;
		}

		switch (curr_char) {
			case EOF:
				return tokens;

			case '(':
				tokens.push_back(p_token(new begin_list));
				continue;

			case ')':
				tokens.push_back(p_token(new end_list));
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
