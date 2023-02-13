#ifndef _stran_parse_h_
#define _stran_parse_h_

#include "pair.h"

namespace stran {

	tok_list lex(std::istream &is);

	sp<datum> parse_datum(const tok_list &toks, size_t &idx);

}

#endif
