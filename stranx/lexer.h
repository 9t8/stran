#ifndef _stranx_lexer_h_
#define _stranx_lexer_h_

#include "object.h"

struct lexer {
	lexer(std::istream &i) : is(i), row(1), col(0) {}

	token_list lex();

private:
	int get();

	std::istream &is;

	size_t row;

	size_t col;
};

#endif
