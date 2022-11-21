#ifndef _stranex_eval_h_
#define _stranex_eval_h_

#include "datum.h"

void eval(std::vector<std::shared_ptr<const datum>> &trees, std::ostream &os);

#endif
