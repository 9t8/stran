#ifndef _stranx_procedure_h_
#define _stranx_procedure_h_

#include "datum.h"

#include <sstream>
#include <vector>

struct procedure : function {
	procedure(const std::vector<std::string> &fs, const bool &v, const p_pair &b,
			  const_p_env &p_e) : formals(fs), variadic(v), body(b), env(p_e) {
		assert(!variadic || !fs.empty() && "procedure taking no arguments cannot be variadic");
	}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "#procedure@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, const_p_env &) const override;

private:
	const_p_env make_new_env(const p_datum &args) const;

	const std::vector<std::string> formals;

	const bool variadic;

	const p_pair body;

	const_p_env env;
};

template <class func> struct native_function : function {
	native_function(const func &cf) : call_func(cf) {}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "#native_function@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, const_p_env &env) const override {
		return call_func(args, env);
	}

private:
	const func call_func;
};

#endif
