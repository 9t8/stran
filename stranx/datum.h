#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include "token.h"

#include <cassert>
#include <unordered_map>

namespace stranx {

	struct context;

	struct datum : token, std::enable_shared_from_this<datum> {
		// self-evaluating by default
		virtual sp<datum> eval(const sp<context> &) {
			return shared_from_this();
		}
	};

	struct context {
		context(const sp<context> &p) : parent(p) {}

		const sp<datum> &find(const std::string &name);

		void define(const std::string &name, const sp<datum> &val) {
			table[name] = val;
		}

	private:
		std::unordered_map<std::string, sp<datum>> table;

		const sp<context> parent;
	};

	struct function : datum {
		virtual sp<datum> call(const sp<datum> &args, const sp<context> &ctx) const = 0;
	};

	struct emptyl : datum {
		operator std::string() const override {
			return "()";
		}

		sp<datum> eval(const sp<context> &) override {
			assert(0 && "attempted to evaluate empty list");
			throw;
		}
	};

	struct pair : datum {
		static bool stringify_into_lists;

		pair(const sp<datum> &a, const sp<datum> &b = std::make_shared<emptyl>())
				: car(a) , cdr(b) {}

		operator std::string() const override;

		sp<datum> eval(const sp<context> &ctx) override {
			const sp<function> func(
				std::dynamic_pointer_cast<function>(car->eval(ctx)));
			assert(func && "attemped to call an uncallable object");

			return func->call(cdr, ctx);
		}

		sp<datum> car, cdr;
	};

	const sp<datum> &next(sp<pair> &exprs);

	struct iden : datum {
		iden(const std::string &n) : name(n) {}

		operator std::string() const override {
			return name;
		}

		sp<datum> eval(const sp<context> &ctx) override {
			return ctx->find(name);
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

}

#endif
