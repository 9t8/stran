#ifndef _stranex_interpret_h_
#define _stranex_interpret_h_

#include "token.h"

#include <unordered_map>

using env_type = std::unordered_map<std::string, std::shared_ptr<token>>;

std::shared_ptr<token> interpret_next_expression(const std::unique_ptr<token> &p_ast,
		env_type &env);

#endif
