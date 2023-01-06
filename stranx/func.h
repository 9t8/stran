#ifndef _stranx_func_h_
#define _stranx_func_h_

#include "datum.h"

#include <sstream>
#include <vector>

namespace stranx {

	struct func : datum {
		virtual sp<datum> call(const sp<datum> &args, const sp<env> &curr_env) const = 0;
	};

	struct pair : datum {
		static bool stringify_into_lists;

		pair(const sp<datum> &a, const sp<datum> &b = std::make_shared<emptyl>())
				: car(a) , cdr(b) {}

		operator std::string() const override;

		sp<datum> car, cdr;

	private:
		sp<datum> internal_eval(const sp<env> &curr_env) override {
			const sp<func> p_func(std::dynamic_pointer_cast<func>(eval(car, curr_env)));
			assert(p_func && "attemped to call an uncallable object");

			return p_func->call(cdr, curr_env);
		}
	};

	const sp<datum> &next(sp<pair> &exprs);

	struct native_func : func {
		native_func(sp<datum> (*const p_f)(const sp<datum> &args, const sp<env> &curr_env))
				: p_func(p_f) {}

		operator std::string() const override {
			std::ostringstream oss;
			oss << "#native_function@" << this;
			return oss.str();
		}

		sp<datum> call(const sp<datum> &args, const sp<env> &curr_env) const override {
			return p_func(args, curr_env);
		}

	private:
		sp<datum> (*const p_func)(const sp<datum> &args, const sp<env> &curr_env);
	};

	struct closure : func {
		closure(const std::vector<std::string> &fs, const bool &v, const sp<pair> &b,
				const sp<env> &c) : formals(fs), variadic(v), body(b), context(c) {
			assert(!(variadic && formals.empty()) &&
				   "procedure taking no arguments cannot be variadic");
			assert(context && "procedure must have a context");
		}

		operator std::string() const override {
			std::ostringstream oss;
			oss << "#procedure@" << this;
			return oss.str();
		}

		sp<datum> call(const sp<datum> &args, const sp<env> &curr_env) const override;

	private:
		const std::vector<std::string> formals;

		const bool variadic;

		const sp<pair> body;

		const sp<env> context;
	};

}

#endif
