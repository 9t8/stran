#ifndef _stranex_lex_h_
#define _stranex_lex_h_

#include "token.h"

#include <deque>

std::deque<std::unique_ptr<token>> lex(std::istream &in);

#endif
