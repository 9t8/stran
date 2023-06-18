#ifndef _stran_pair_h_
#define _stran_pair_h_

#include <sstream>
#include <vector>

#include "datum.h"

namespace stran {

sp<datum> next(sp<datum> &args);

inline sp<datum> eval_next(sp<datum> &args, const sp<env> &curr_env) {
  return eval(next(args), curr_env);
}

struct func : datum {
  virtual sp<datum> call(sp<datum> args, const sp<env> &curr_env) const = 0;
};

struct pair : datum {
  pair(const sp<datum> &a, const sp<datum> &b = nullptr) : car(a), cdr(b) {}

  sp<datum> car, cdr;

private:
  operator std::string() const override;

  sp<datum> internal_eval(const sp<env> &curr_env) override {
    const sp<func> p_func(sp_cast<func>(eval(car, curr_env)));
    assert(p_func && "attemped to call an uncallable object");

    return p_func->call(cdr, curr_env);
  }
};

struct native_func : func {
  native_func(sp<datum> (*const p_f)(sp<datum> &args, const sp<env> &curr_env))
      : p_func(p_f) {}

  sp<datum> call(sp<datum> args, const sp<env> &curr_env) const override {
    return p_func(args, curr_env);
  }

private:
  operator std::string() const override {
    std::ostringstream oss;
    oss << "#native_function@" << this;
    return oss.str();
  }

  sp<datum> (*const p_func)(sp<datum> &args, const sp<env> &curr_env);
};

struct closure : func {
  closure(const std::vector<std::string> &fs, const bool v, const sp<pair> &b,
          const sp<env> &c)
      : formals(fs), variadic(v), body(b), context(c) {
    assert(!(variadic && formals.empty()) &&
           "nullary procedure cannot be variadic");
    assert(body && "procedure body cannot be empty");
    assert(context && "procedure must have a context");
  }

  sp<datum> call(sp<datum> args, const sp<env> &curr_env) const override;

private:
  operator std::string() const override {
    std::ostringstream oss;
    oss << "#procedure@" << this;
    return oss.str();
  }

  const std::vector<std::string> formals;

  const bool variadic;

  const sp<pair> body;

  const sp<env> context;
};

} // namespace stran

#endif
