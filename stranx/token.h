#ifndef _stranx_token_h_
#define _stranx_token_h_

#include "datum.h"

#include <deque>

struct token;

typedef std::unique_ptr<token> p_token;

typedef std::deque<p_token> token_list;

struct token : object {
	// parses to itself by default
	virtual p_datum parse(token_list &tokens) const {
		assert(tokens.at(0).get() == this && "first token in tokens is not current token");

		p_datum p(dynamic_cast<const datum *>(tokens.front().release()));
		assert(p != nullptr && "this token is not a datum");

		tokens.pop_front();
		return p;
	}
};

struct begin_list : token {
	operator std::string() const override {
		return "(";
	}

	p_datum parse(token_list &tokens) const override;
};

struct end_list : token {
	operator std::string() const override {
		return ")";
	}

	p_datum parse(token_list &tokens) const override {
		assert(0 && "unexpected end of list token");
		throw; // suppress warning
	}
};

struct dot : token, datum {
	operator std::string() const override {
		return ".";
	}

	p_datum parse(token_list &tokens) const override {
		token &next_next(*tokens.at(2));
		token &next(*tokens[1]);
		assert(typeid(next) != typeid(end_list) && typeid(next_next) == typeid(end_list) &&
			   "illegal location for dot token");

		return token::parse(tokens);
	}

	p_datum eval(environment &env) const override {
		assert(0 && "illegal dot token");
		throw;
	}
};

struct identifier : token, datum {
	identifier(const std::string &n) : name(n) {}

	operator std::string() const override {
		return name;
	}

	p_datum eval(environment &env) const override {
		environment::iterator it(env.find(name));
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
