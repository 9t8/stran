#ifndef _stranx_native_function_h_
#define _stranx_native_function_h_

#include "datum.h"

#include <sstream>

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
