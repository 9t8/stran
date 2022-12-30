#ifndef _stranx_datum_h_
#define _stranx_datum_h_

#include "tok.h"

#include <cassert>
#include <unordered_map>

namespace stranx {

	struct env;

	struct datum : tok, std::enable_shared_from_this<datum> {
		// self-evaluating by default
		virtual sp<datum> eval(const sp<env> &) {
			return shared_from_this();
		}
	};

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

	struct quote : datum {
		quote(const sp<datum> &c) : contents(c) {}

		operator std::string() const override {
			return "'" + static_cast<std::string>(*contents);
		}

		sp<datum> eval(const sp<env> &) override {
			return contents;
		}

	private:
		sp<datum> contents;
	};

	struct emptyl : datum {
		operator std::string() const override {
			return "()";
		}

		sp<datum> eval(const sp<env> &) override {
			assert(0 && "attempted to evaluate empty list");
			throw;
		}
	};

	struct iden : datum {
		iden(const std::string &n) : name(n) {}

		operator std::string() const override {
			return name;
		}

		sp<datum> eval(const sp<env> &curr_env) override {
			return curr_env->find(name);
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
