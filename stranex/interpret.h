#ifndef _stranex_interpret_h_
#define _stranex_interpret_h_

#include "datum.h"

void interpret(std::vector<std::unique_ptr<datum>> &trees, std::ostream &os);

#endif
