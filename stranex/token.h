#ifndef _stranex_token_h_
#define _stranex_token_h_

#include <string>
#include <vector>

struct token {
	virtual ~token() {}

	virtual operator std::string() const = 0;

	friend std::ostream &operator<<(std::ostream &os, const token &t) {
		return os << static_cast<std::string>(t);
	}
};

// token list only
struct beginl : public token {
	operator std::string() const override {
		return "(";
	}
};

// token list only
struct endl : public token {
	operator std::string() const override {
		return ")";
	}
};

// syntax tree only
struct list : public token {
	operator std::string() const override;

	std::vector<std::unique_ptr<token>> elements;
};

// environmental only
struct pair : public token {
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
