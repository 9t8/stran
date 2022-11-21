#ifndef _stranex_parse_h_
#define _stranex_parse_h_

#include "datum.h"

#include <deque>

// destroys tokens
std::vector<std::shared_ptr<const datum>> parse(std::deque<std::unique_ptr<token>> &tokens);

#endif
