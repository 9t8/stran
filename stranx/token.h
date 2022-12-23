#ifndef _stranx_token_h_
#define _stranx_token_h_

#include "datum.h"

#include <deque>

typedef std::deque<std::unique_ptr<object>> token_list;

struct atom : object {
	atom(const p_datum &p_d) : val(p_d) {}

	operator std::string() const override {
		return *val;
	}

	const p_datum val;
};

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
