#ifndef _stranx_closure_h_
#define _stranx_closure_h_

#include "datum.h"

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

		sp<datum> call(const sp<datum> &args, const sp<env> &p_e) const override;

	private:
		const std::vector<std::string> formals;

		const bool variadic;

		const sp<pair> body;

		const sp<env> context;
	};

	struct lambda : func {
		operator std::string() const override {
			std::ostringstream oss;
			oss << "#lambda@" << this;
			return oss.str();
		}

		sp<datum> call(const sp<datum> &args, const sp<env> &p_e) const override;
	};

	struct define : func {
		operator std::string() const override {
			std::ostringstream oss;
			oss << "#define@" << this;
			return oss.str();
		}

		sp<datum> call(const sp<datum> &args, const sp<env> &p_e) const override;
	};

}

#endif
