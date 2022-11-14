#ifndef _stranex_object_h_
#define _stranex_object_h_

#include <string>
#include <vector>
#include <deque>
#include <cassert>
#include <unordered_map>

struct datum;
using env_type = std::unordered_map<std::string, std::shared_ptr<datum>>;

struct object {
	virtual ~object() {}

	virtual operator std::string() const = 0;

	friend std::ostream &operator<<(std::ostream &os, const object &t) {
		return os << static_cast<std::string>(t);
	}
};

struct datum : public object {
	//virtual std::shared_ptr<object> eval(env_type &env) = 0;
};

struct token : public object {
	virtual std::unique_ptr<datum> parse(std::deque<std::unique_ptr<token>> &tokens) const {
		std::unique_ptr<datum> p(dynamic_cast<datum *>(tokens.front().release()));
		assert(p != nullptr && "tokens.front() is not a datum");
		tokens.pop_front();
		return p;
	}
};

struct begin_list : public token {
	operator std::string() const override {
		return "(";
	}

	std::unique_ptr<datum> parse(std::deque<std::unique_ptr<token>> &tokens) const override;
};

struct end_list : public token {
	operator std::string() const override {
		return ")";
	}

	std::unique_ptr<datum> parse(std::deque<std::unique_ptr<token>> &tokens) const override {
		assert(0 && "unexpected end of list token");
		throw; // suppress warning
	}
};

struct identifier : public token, public datum {
	identifier(const std::string &n) : name(n) {}

	operator std::string() const override {
		return name;
	}

	std::string name;
};

struct list : public datum {
	operator std::string() const override;

	std::vector<std::unique_ptr<object>> elements;
};

// currently unused
struct pair : public object {
	static bool stringify_into_lists;

	operator std::string() const override;

	std::shared_ptr<token> car, cdr;
};

struct decimal : public token, public datum {
	decimal(const double &v) : val(v) {}

	operator std::string() const override {
		return std::to_string(val);
	}

	double val;
};

#endif
