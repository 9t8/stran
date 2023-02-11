#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include <cassert>
#include <unordered_map>

#include "tok.h"

namespace stranx {

	struct datum;

	struct env {
		env(const sp<env> &p) : parent(p) {}

		const sp<datum> &find(const std::string &name) const;

		void define(const std::string &name, const sp<datum> &val) {
			table[name] = val;
		}

	private:
		std::unordered_map<std::string, sp<datum>> table;

		const sp<env> parent;
	};

	struct datum : tok, std::enable_shared_from_this<datum> {
		friend sp<datum> eval(const sp<datum> &p_d, const sp<env> &curr_env) {
			assert(p_d && "attempted to evaluate empty list");
			return p_d->internal_eval(curr_env);
		}

	private:
		// self-evaluating by default
		virtual sp<datum> internal_eval(const sp<env> &) {
			return shared_from_this();
		}
	};

	struct iden : datum {
		iden(const std::string &n) : name(n) {}

		const std::string name;

	private:
		operator std::string() const override {
			return name;
		}

		sp<datum> internal_eval(const sp<env> &curr_env) override {
			return curr_env->find(name);
		}
	};

	struct inexact : datum {
		inexact(const double v) : val(v) {}

	private:
		operator std::string() const override {
			return std::to_string(val);
		}

		const double val;
	};

}

#endif
