#ifndef _stranx_closure_h_
#define _stranx_closure_h_

#include "datum.h"

#include <sstream>
#include <vector>

namespace stranx {

	struct closure : func {
		closure(const std::vector<std::string> &fs, const bool &v, const sp<pair> &b,
				const sp<context> &p_i) : formals(fs), variadic(v), body(b), internal_ctx(p_i) {
			assert(!variadic || !formals.empty() &&
				   "procedure taking no arguments cannot be variadic");
			assert(internal_ctx && "procedure must have a context");
		}

		operator std::string() const override {
			std::ostringstream oss;
			oss << "#procedure@" << this;
			return oss.str();
		}

		sp<datum> call(const sp<datum> &args, const sp<context> &ctx) const override;

	private:
		const sp<context> create_new_ctx(const sp<datum> &args, const sp<context> &ctx) const;

		const std::vector<std::string> formals;

		const bool variadic;

		const sp<pair> body;

		const sp<context> internal_ctx;
	};

	struct lambda : func {
		operator std::string() const override {
			std::ostringstream oss;
			oss << "#lambda@" << this;
			return oss.str();
		}

		sp<datum> call(const sp<datum> &args, const sp<context> &ctx) const override;
	};

	struct define : func {
		operator std::string() const override {
			std::ostringstream oss;
			oss << "#define@" << this;
			return oss.str();
		}

		sp<datum> call(const sp<datum> &args, const sp<context> &ctx) const override;
	};

}

#endif
