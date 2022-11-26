#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include "object.h"

#include <cassert>
#include <vector>
#include <unordered_map>
#include <sstream>

struct datum;

typedef std::shared_ptr<const datum> p_datum;

typedef std::unordered_map<std::string, p_datum> environment;

struct datum : object, std::enable_shared_from_this<datum> {
	// self evaluating by default
	virtual p_datum eval(environment &env) const {
		return shared_from_this();
	}
};

struct function : datum {
	function(const std::vector<std::string> &fs) : formals(fs) {}

	// todo change here to allow varargs
	virtual p_datum call(
		environment &env, const std::vector<p_datum> &args
	) const final {
		assert(args.size() == formals.size() && "wrong number of arguments");
		return internal_call(env, args);
	}

protected:
	std::vector<std::string> formals;

private:
	virtual p_datum internal_call(
		environment &env, const std::vector<p_datum> &args
	) const = 0;
};

struct procedure : function {
	procedure(const std::vector<std::string> &formals, const p_datum &b)
			: function(formals), body(b) {}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "procedure@" << this;
		return oss.str();
	}

private:
	p_datum internal_call(
		environment &env, const std::vector<p_datum> &args
	) const override;

	p_datum body;
};

struct native_function : function {
	native_function(
		const std::vector<std::string> &formals,
		p_datum(*p_f)(environment &env, const std::vector<p_datum> &args)
	) : function(formals), p_func(p_f) {}

	operator std::string() const override {
		std::ostringstream oss;
		oss << "native_function@" << this;
		return oss.str();
	}

private:
	p_datum internal_call(
		environment &env, const std::vector<p_datum> &args
	) const override {
		return p_func(env, args);
	}

	// todo: change function pointer to std::function or template argument
	p_datum(*p_func)(environment &env, const std::vector<p_datum> &args);
};

struct list : datum {
	operator std::string() const override;

	p_datum eval(environment &env) const override {
		assert(!elements.empty() && "expression is empty");

		// throws if first element is not a function
		return dynamic_cast<const function &>(*elements[0]->eval(env))
			   .call(env, std::vector<p_datum>(elements.begin() + 1, elements.end()));
	}

	std::vector<p_datum> elements;
};

#endif
