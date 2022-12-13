#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include "object.h"

#include <cassert>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <iostream>

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
	std::vector<std::string> formals;

private:
	environment create_new_env(const p_datum &args, environment &env) const;

	p_datum body;

	bool variadic;
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
	lambda_type lambda;
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

	pair() : car(std::make_shared<empty_list>()) , cdr(std::make_shared<empty_list>()) {}

	operator std::string() const override;

	p_datum eval(environment &env) override {
		p_datum func(dynamic_cast<const function *>(car->eval(env)));
		std::cerr << typeid(*func).name() << "\n" << typeid(function).name() << "\n";
		assert(typeid(*func) == typeid(function) && "attemped to call an uncallable object");
		
		return dynamic_cast<const function &>(*func).call(cdr, env); // issue here
	}

	p_datum car, cdr;
};

#endif
