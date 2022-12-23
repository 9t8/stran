#ifndef _stranx_token_h_
#define _stranx_token_h_

#include "datum.h"

#include <deque>

struct token;

typedef std::deque<std::unique_ptr<token>> token_list;

struct token : object {
	virtual p_datum parse(const token_list &tokens, size_t &idx) const = 0;
};

struct begin_list : token {
	operator std::string() const override {
		return "(";
	}

	p_datum parse(const token_list &tokens, size_t &idx) const override;
};

struct end_list : token {
	operator std::string() const override {
		return ")";
	}

	p_datum parse(const token_list &, size_t &) const override {
		assert(0 && "unexpected end of list token");
		throw;
	}
};

struct dot : token {
	operator std::string() const override {
		return ".";
	}

	p_datum parse(const token_list &, size_t &) const override {
		assert(0 && "unexpected dot token");
		throw;
	}
};

struct atom : token {
	atom(const p_datum &p_d) : val(p_d) {}

	operator std::string() const override {
		return *val;
	}

	p_datum parse(const token_list &tokens, size_t &idx) const override {
		assert(tokens.at(idx).get() == this && "first token in tokens is not current token");

		++idx;
		return val;
	}

private:
	const p_datum val;
};

#endif
