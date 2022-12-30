#include "lexer.h"

#include "datum.h"

#include <sstream>
#include <vector>

using namespace stranx;

tok_list lexer::lex() {
	tok_list toks;

	std::string curr_word;
	for (;;) {
		int curr_char(get());

		switch (curr_char) {
			case ' ':
			case '\n':
			case '\t':
			case EOF:
			case '(':
			case ')':
			case ';':
			case '\'':
				break;

			default:
				curr_word += static_cast<char>(curr_char);
				continue;
		}

		if (!curr_word.empty()) {
			if (curr_word == ".") {
				toks.push_back(std::make_shared<dot>());
			} else
				if (curr_word[0] == '+' || curr_word[0] == '-' || curr_word[0] == '.' ||
					(curr_word[0] >= '0' && curr_word[0] <= '9')) {
					size_t idx(0);
					double val(stod(curr_word, &idx));
					assert(idx == curr_word.size() &&
						   "invalid character while parsing decimal");
					toks.push_back(std::make_shared<decimal>(val));
				} else {
					toks.push_back(std::make_shared<iden>(curr_word));
				}
			curr_word.clear();
		}

		switch (curr_char) {
			case EOF:
				return toks;

			case '(':
				toks.push_back(std::make_shared<beginl>());
				break;

			case ')':
				toks.push_back(std::make_shared<endl>());
				break;

			case ';':
				while (curr_char != '\n' && curr_char != EOF) {
					curr_char = get();
				}
				break;
		}
	}
}

int lexer::get() {
	if (is.peek() == '\n') {
		col = 0;
		++row;
	} else {
		++col;
	}
	return is.get();
}
