#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include "object.h"

#include <cassert>
#include <sstream>
#include <unordered_map>
#include <vector>

struct datum;

typedef std::shared_ptr<datum> p_datum;

typedef std::unordered_map<std::string, p_datum> environment;

struct datum : object, std::enable_shared_from_this<datum> {
	// self evaluating by default
	virtual p_datum eval(environment &env) {
		return shared_from_this();
	}
};

struct function : datum {
	virtual p_datum call(const p_datum &args, environment &env) const = 0;
};

inline p_datum find(const std::string &name, const environment &env) {
	environment::const_iterator it(env.find(name));
	assert(it != env.end() && "undefined identifier");

	return it->second;
}

inline p_datum call(const p_datum &func, const p_datum &args, environment &env) {
	assert(dynamic_cast<const function *>(func.get()) != nullptr &&
		   "attemped to call an uncallable object");

	return dynamic_cast<const function &>(*func).call(args, env);
}

struct procedure : function {
	procedure(const std::vector<std::string> &fs, const p_datum &b, const bool &v)
			: formals(fs), body(b), variadic(v) {
		assert(!(variadic && fs.empty()) && "procedure taking no arguments cannot be variadic");
	}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "procedure@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, environment &env) const override;

protected:
	const std::vector<std::string> formals;

private:
	environment create_new_env(const p_datum &args, environment &env) const;

	const p_datum body;

	const bool variadic;
};

template <class lambda_type> struct native_function : function {
	native_function(const lambda_type &l) : lambda(l) {}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "native_function@" << this;
		return oss.str();
	}

	p_datum call(const p_datum &args, environment &env) const override {
		return lambda(args, env);
	}

private:
	const lambda_type lambda;
};

struct empty_list : datum {
	operator std::string() const override {
		return "()";
	}

	p_datum eval(environment &env) override {
		assert(0 && "attempted to evaluate empty list");
		throw;
	}
};

struct pair : datum {
	static bool stringify_into_lists;

	pair(const p_datum &a, const p_datum &b = std::make_shared<empty_list>())
			: car(a) , cdr(b) {}

	operator std::string() const override;

	p_datum eval(environment &env) override {
		return call(car->eval(env), cdr, env);
	}

	p_datum car, cdr;
};

#endif
