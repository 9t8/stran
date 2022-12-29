#ifndef _stranx_closure_h_
#define _stranx_closure_h_

#include "datum.h"

#include <sstream>
#include <vector>

namespace stranx {

	struct closure : function {
		closure(const std::vector<std::string> &fs, const bool &v, const p_pair &b,
				const p_ctx &p_i) : formals(fs), variadic(v), body(b), internal_ctx(p_i) {
			assert(!variadic || !formals.empty() &&
				   "procedure taking no arguments cannot be variadic");
			assert(internal_ctx && "procedure must have a context");
		}

		operator std::string() const override {
			std::ostringstream oss;
			oss << "#procedure@" << this;
			return oss.str();
		}

		p_datum call(const p_datum &args, const p_ctx &ctx) const override;

	private:
		const p_ctx make_new_ctx(const p_datum &args, const p_ctx &ctx) const;

		const std::vector<std::string> formals;

		const bool variadic;

		const p_pair body;

		const p_ctx internal_ctx;
	};

	struct lambda : function {
		operator std::string() const override {
			std::ostringstream oss;
			oss << "#lambda@" << this;
			return oss.str();
		}

		p_datum call(const p_datum &args, const p_ctx &ctx) const override;
	};

	struct define : function {
		operator std::string() const override {
			std::ostringstream oss;
			oss << "#define@" << this;
			return oss.str();
		}

		p_datum call(const p_datum &args, const p_ctx &ctx) const override;
	};

}

#endif
