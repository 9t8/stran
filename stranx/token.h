#ifndef _stranx_token_h_
#define _stranx_token_h_

#include "datum.h"

#include <deque>

struct token;

typedef std::deque<std::unique_ptr<token>> token_list;

struct token : object {
	virtual p_datum parse(token_list &tokens) const = 0;
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

	p_datum parse(token_list &) const override {
		assert(0 && "unexpected end of list token");
		throw;
	}
};

struct dot : token {
	operator std::string() const override {
		return ".";
	}

	p_datum parse(token_list &) const override {
		assert(0 && "unexpected dot token");
		throw;
	}
};

struct atom : token {
	atom(const p_datum &p_d) : val(p_d) {}

	operator std::string() const override {
		return *val;
	}

	p_datum parse(token_list &tokens) const override {
		assert(tokens.at(0).get() == this && "first token in tokens is not current token");

		p_datum v(val);
		tokens.pop_front();
		return v;
	}

private:
	const p_datum val;
};

#endif
