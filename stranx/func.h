#ifndef _stranx_func_h_
#define _stranx_func_h_

#include "datum.h"

namespace stranx {

	struct func : datum {
		virtual sp<datum> call(const sp<datum> &args, const sp<env> &curr_env) const = 0;
	};

	struct pair : datum {
		static bool stringify_into_lists;

		pair(const sp<datum> &a, const sp<datum> &b = std::make_shared<emptyl>())
				: car(a) , cdr(b) {}

		operator std::string() const override;

		sp<datum> eval(const sp<env> &curr_env) override {
			const sp<func> p_func(std::dynamic_pointer_cast<func>(car->eval(curr_env)));
			assert(p_func && "attemped to call an uncallable object");

			return p_func->call(cdr, curr_env);
		}

		sp<datum> car, cdr;
	};

	const sp<datum> &next(sp<pair> &exprs);

}

#endif
