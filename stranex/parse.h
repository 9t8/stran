#ifndef _stranex_parse_h_
#define _stranex_parse_h_

#include "object.h"

#include <deque>

// destroys tokens
std::vector<std::unique_ptr<object>> parse(std::deque<std::unique_ptr<token>> &tokens);

#endif
