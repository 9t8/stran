#ifndef _stranex_datum_h_
#define _stranex_datum_h_

#include "object.h"

#include <vector>
#include <deque>
#include <cassert>
#include <unordered_map>

struct datum;
typedef std::unordered_map<std::string, std::shared_ptr<const datum>> env_type;

struct datum : public object {
	// self-evaluating by default
	virtual std::shared_ptr<const datum> eval(env_type &env) const {
		return std::shared_ptr<const datum>(this);
	}
};

struct token : public object {
	virtual std::shared_ptr<const datum> parse(std::deque<std::unique_ptr<token>> &tokens) const {
		assert(tokens.front().get() == this && "tokens.front is not current token");
		std::shared_ptr<const datum> p(dynamic_cast<const datum *>(tokens.front().release()));
		assert(p != nullptr && "tokens.front() is not a datum");
		tokens.pop_front();
		return p;
	}
};

struct begin_list : public token {
	operator std::string() const override {
		return "(";
	}

	std::shared_ptr<const datum> parse(std::deque<std::unique_ptr<token>> &tokens) const override;
};

struct end_list : public token {
	operator std::string() const override {
		return ")";
	}

	std::shared_ptr<const datum> parse(std::deque<std::unique_ptr<token>> &tokens) const override {
		assert(0 && "unexpected end of list token");
		throw; // suppress warning
	}
};

struct identifier : public token, public datum {
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

struct procedure : public datum {
	virtual std::shared_ptr<const datum> call(
		env_type &env, std::vector<std::unique_ptr<const datum>> &args
	) const;

	std::vector<std::string> params;

	std::unique_ptr<const datum> body;
};

struct list : public datum {
	operator std::string() const override;

	std::vector<std::shared_ptr<const datum>> elements;
};

struct decimal : public token, public datum {
	decimal(const double &v) : val(v) {}

	operator std::string() const override {
		return std::to_string(val);
	}

	double val;
};

// unused

struct pair : public object {
	static bool stringify_into_lists;

	operator std::string() const override;

	std::shared_ptr<token> car, cdr;
};

#endif
