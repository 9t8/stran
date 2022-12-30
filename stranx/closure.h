#ifndef _stranx_closure_h_
#define _stranx_closure_h_

#include "func.h"

#include <sstream>
#include <vector>

namespace stranx {

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

}

#endif
