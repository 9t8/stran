#ifndef _stranex_interpret_h_
#define _stranex_interpret_h_

#include "object.h"

#include <unordered_map>

using env_type = std::unordered_map<std::string, std::shared_ptr<object>>;

void interpret(std::vector<std::unique_ptr<object>> &trees, std::ostream &os);

#endif
