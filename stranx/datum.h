#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include "object.h"

#include <cassert>
#include <unordered_map>

struct datum;
typedef std::shared_ptr<datum> p_datum;

struct environment;
typedef std::shared_ptr<environment> p_env;

struct datum : object, std::enable_shared_from_this<datum> {
	// self-evaluating by default
	virtual p_datum eval(const p_env &) {
		return shared_from_this();
	}
};

struct environment {
	environment(const p_env &p) : parent(p) {}

	const p_datum &find(const std::string &name);

	void define(const std::string &name, const p_datum &val) {
		table[name] = val;
	}

private:
	std::unordered_map<std::string, p_datum> table;

	const p_env parent;
};

struct function : datum {
	virtual p_datum call(const p_datum &args, const p_env &env) const = 0;
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

struct pair : datum {
	static bool stringify_into_lists;

	pair(const p_datum &a, const p_datum &b = std::make_shared<empty_list>())
			: car(a) , cdr(b) {}

	operator std::string() const override;

	p_datum eval(const p_env &env) override {
		const p_datum func(car->eval(env));
		assert(dynamic_cast<const function *>(func.get()) &&
			   "attemped to call an uncallable object");

		return dynamic_cast<const function &>(*func).call(cdr, env);
	}

	p_datum car, cdr;
};

typedef std::shared_ptr<pair> p_pair;

const p_datum &next(p_pair &exprs);

struct identifier : datum {
	identifier(const std::string &n) : name(n) {}

	operator std::string() const override {
		return name;
	}

	p_datum eval(const p_env &env) override {
		return env->find(name);
	}

	const std::string name;
};

struct decimal : datum {
	decimal(const double &v) : val(v) {}

	operator std::string() const override {
		return std::to_string(val);
	}

private:
	const double val;
};

#endif
