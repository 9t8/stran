#ifndef _stranex_parse_h_
#define _stranex_parse_h_

#include "token.h"

#include <deque>

// destroys tokens
std::vector<std::unique_ptr<token>> parse(std::deque<std::unique_ptr<token>> &tokens);

#endif
