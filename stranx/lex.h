#ifndef _stranx_lex_h_
#define _stranx_lex_h_

#include "func.h"

namespace stranx {

	tok_list lex(std::istream &is);

	sp<datum> parse_datum(const tok_list &toks, size_t &idx);

}

#endif
