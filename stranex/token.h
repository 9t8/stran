#ifndef _stranex_token_h_
#define _stranex_token_h_

#include "datum.h"

#include <deque>
#include <cassert>

struct token;

typedef std::deque<std::unique_ptr<token>> token_list;

struct token : object {
	// parses to itself by default
	virtual std::shared_ptr<const datum> parse(token_list &tokens) const {
		assert(tokens.front().get() == this && "tokens.front is not current token");
		std::shared_ptr<const datum> p(dynamic_cast<const datum *>(tokens.front().release()));
		assert(p != nullptr && "tokens.front() is not a datum");
		tokens.pop_front();
		return p;
	}
};

struct begin_list : token {
	operator std::string() const override {
		return "(";
	}

	std::shared_ptr<const datum> parse(token_list &tokens) const override;
};

struct end_list : token {
	operator std::string() const override {
		return ")";
	}

	std::shared_ptr<const datum> parse(token_list &tokens) const override {
		assert(0 && "unexpected end of list token");
		throw; // suppress warning
	}
};

struct identifier : token, datum {
	identifier(const std::string &n) : name(n) {}

	operator std::string() const override {
		return name;
	}

	std::shared_ptr<const datum> eval(env_type &env) const override {
		env_type::iterator it(env.find(name));
		assert(it != env.end() && "undefined identifier");
		return it->second;
	}

	std::string name;
};

struct decimal : token, datum {
	decimal(const double &v) : val(v) {}

	operator std::string() const override {
		return std::to_string(val);
	}

	double val;
};

#endif
