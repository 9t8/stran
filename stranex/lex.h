#ifndef _stranex_lex_h_
#define _stranex_lex_h_

#include "object.h"

#include <deque>

std::deque<std::unique_ptr<token>> lex(std::istream &is);

#endif
