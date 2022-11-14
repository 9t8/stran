#ifndef _stranex_interpret_h_
#define _stranex_interpret_h_

#include "object.h"

void interpret(std::vector<std::unique_ptr<datum>> &trees, std::ostream &os);

#endif
