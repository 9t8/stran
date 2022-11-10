#ifndef _stranex_token_h_
#define _stranex_token_h_

#include <sstream>
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

// pre-cons tree only
struct list : public token {
	operator std::string() const override {
		std::ostringstream oss;
		oss << "[";
	if (!elements.empty()) {
	oss << *elements.front();
		for (int i(1); i < elements.size(); ++i) {
			oss << " " << *elements[i];
		}
	}
	oss << "]";
	return oss.str();
	}

	std::vector<std::unique_ptr<token>> elements;
};

// post-cons tree and later only
struct pair : public token {
	operator std::string() const override {
		return "(" + static_cast<std::string>(*car)
			   + " . " + static_cast<std::string>(*cdr) + ")";
	}

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
