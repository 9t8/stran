#ifndef _stranx_token_h_
#define _stranx_token_h_

#include "object.h"

#include <deque>

typedef std::deque<std::shared_ptr<object>> token_list;

struct begin_list : object {
	operator std::string() const override {
		return "(";
	}
};

struct end_list : object {
	operator std::string() const override {
		return ")";
	}
};

struct dot : object {
	operator std::string() const override {
		return ".";
	}
};

#endif
