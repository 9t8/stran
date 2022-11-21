#ifndef _stranex_datum_h_
#define _stranex_datum_h_

#include "object.h"

#include <vector>
#include <unordered_map>

struct datum;

typedef std::unordered_map<std::string, std::shared_ptr<const datum>> env_type;

typedef std::vector<std::shared_ptr<const datum>> syntax_tree;

struct datum : object, std::enable_shared_from_this<datum> {
	// self-evaluating by default
	virtual std::shared_ptr<const datum> eval(env_type &env) const {
		return shared_from_this();
	}
};

struct procedure : datum {
	virtual std::shared_ptr<const datum> call(
		env_type &env, std::vector<std::unique_ptr<const datum>> &args
	) const;

	std::vector<std::string> params;

	std::unique_ptr<const datum> body;
};

struct list : datum {
	operator std::string() const override;

	syntax_tree elements;
};

// token stuff

#include <deque>
#include <cassert>

struct token;

typedef std::deque<std::unique_ptr<token>> token_list;

struct token : object {
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
