#ifndef _stranex_object_h_
#define _stranex_object_h_

#include <string>
#include <vector>
#include <deque>
#include <cassert>

struct object {
	virtual ~object() {}

	virtual operator std::string() const = 0;

	friend std::ostream &operator<<(std::ostream &os, const object &t) {
		return os << static_cast<std::string>(t);
	}
};

struct token : public object {
	virtual std::unique_ptr<object> parse(std::deque<std::unique_ptr<token>> &tokens) const {
		std::unique_ptr<token> p_token(std::move(tokens.front()));
		tokens.pop_front();
		return p_token;
	}
};

// token list only
struct beginl : public token {
	operator std::string() const override {
		return "(";
	}

	std::unique_ptr<object> parse(std::deque<std::unique_ptr<token>> &tokens) const override;
};

// token list only
struct endl : public token {
	operator std::string() const override {
		return ")";
	}

	std::unique_ptr<object> parse(std::deque<std::unique_ptr<token>> &tokens) const override {
		assert(0 && "unexpected end of list token");
		throw; // suppress warning
	}
};

// syntax tree only
struct list : public object {
	operator std::string() const override;

	std::vector<std::unique_ptr<object>> elements;
};

// environment only
struct pair : public object {
	static bool stringify_into_lists;

	operator std::string() const override;

	std::shared_ptr<token> car, cdr;
};

struct decimal : public token {
	decimal(const double &v) : val(v) {}

	operator std::string() const override {
		return std::to_string(val);
	}

	double val;
};

struct identifier : public token {
	identifier(const std::string &n) : name(n) {}

	operator std::string() const override {
		return name;
	}

	std::string name;
};

#endif
