#ifndef _stranx_closure_h_
#define _stranx_closure_h_

#include "datum.h"

#include <sstream>
#include <vector>

struct closure : function {
	closure(const std::vector<std::string> &fs, const bool &v, const p_pair &b,
			  const p_env &p_e) : formals(fs), variadic(v), body(b) {
		assert(!variadic || !formals.empty() &&
			   "procedure taking no arguments cannot be variadic");

		set_env(p_e);
	}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "#procedure@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, const p_env &) const override;

	void set_env(const p_env &p_e) {
		assert(p_e && "procedure must have an environment");

		env = p_e;
	}

private:
	const p_env make_new_env(const p_datum &args) const;

	const std::vector<std::string> formals;

	const bool variadic;

	const p_pair body;

	p_env env;
};

struct lambda : function {
	operator std::string() const override {
		std::ostringstream oss;
		oss << "#lambda@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, const p_env &env) const override;
};

struct define : function {
	operator std::string() const override {
		std::ostringstream oss;
		oss << "#define@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, const p_env &env) const override;
};

#endif
