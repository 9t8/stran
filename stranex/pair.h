#ifndef _stranex_pair_h_
#define _stranex_pair_h_

#include "datum.h"

struct pair : object {
	static bool stringify_into_lists;

	operator std::string() const override;

	std::shared_ptr<datum> car, cdr;
};

#endif
