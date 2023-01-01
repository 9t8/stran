#include "lex.h"

#include "datum.h"

#include <istream>
#include <vector>

using namespace stranx;

tok_list stranx::lex(std::istream &is) {
	tok_list toks;

	std::string curr_word;
	for (;;) {
		int curr_char(is.get());

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
				if ((curr_word[0] != '+' && curr_word[0] != '-' && curr_word[0] != '.' &&
					 (curr_word[0] < '0' || curr_word[0] > '9')) ||
					curr_word == "+" || curr_word == "-") {
					toks.push_back(std::make_shared<iden>(curr_word));
				} else {
					size_t idx(0);
					double val(stod(curr_word, &idx));
					assert(idx == curr_word.size() && "invalid character while parsing number");
					toks.push_back(std::make_shared<inexact>(val));
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
					curr_char = is.get();
				}
				break;

			case '\'':
				toks.push_back(std::make_shared<quote_tok>());
				break;
		}
	}
}
