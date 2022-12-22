#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include "object.h"

#include <cassert>
#include <sstream>
#include <unordered_map>
#include <vector>

struct datum;

typedef std::shared_ptr<datum> p_datum;

typedef std::shared_ptr<std::unordered_map<std::string, p_datum>> p_env;

struct datum : object, std::enable_shared_from_this<datum> {
	// self-evaluating by default
	virtual p_datum eval(const p_env &) {
		return shared_from_this();
	}
};

struct function : datum {
	virtual p_datum call(const p_datum &args, const p_env &env) const = 0;
};

template <class func> struct native_function : function {
	native_function(const func &cf) : call_func(cf) {}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "native_function@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, const p_env &env) const override {
		return call_func(args, env);
	}

private:
	const func call_func;
};

struct procedure : function {
	procedure(const std::vector<std::string> &fs, const bool &v, const p_datum &b,
			  const p_env &p_e) : formals(fs), variadic(v), body(b), env(p_e) {
		assert(!variadic || !fs.empty() && "procedure taking no arguments cannot be variadic");
	}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "procedure@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, const p_env &) const override;

private:
	p_env create_new_env(const p_datum &args) const;

	const std::vector<std::string> formals;

	const bool variadic;

	const p_datum body;

	const p_env env;
};

struct empty_list : datum {
	operator std::string() const override {
		return "()";
	}

	p_datum eval(const p_env &) override {
		assert(0 && "attempted to evaluate empty list");
		throw;
	}
};

inline p_datum find(const std::string &name, const p_env &env) {
	p_env::element_type::iterator it(env->find(name));
	assert(it != env->end() && "undefined identifier");

	return it->second;
}

inline p_datum call(const p_datum &func, const p_datum &args, const p_env &env) {
	assert(dynamic_cast<const function *>(func.get()) &&
		   "attemped to call an uncallable object");

	return dynamic_cast<const function &>(*func).call(args, env);
}

struct pair : datum {
	static bool stringify_into_lists;

	pair(const p_datum &a, const p_datum &b = std::make_shared<empty_list>())
			: car(a) , cdr(b) {}

	operator std::string() const override;

	p_datum eval(const p_env &env) override {
		return call(car->eval(env), cdr, env);
	}

	p_datum car, cdr;
};

const p_datum &next(const pair *&exprs);

#endif
