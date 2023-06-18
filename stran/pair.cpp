#include "pair.h"

using namespace stran;

sp<datum> stran::next(sp<datum> &args) {
  assert(args && "not enough args");

  const sp<pair> curr_pair(std::dynamic_pointer_cast<pair>(args));
  assert(curr_pair && "improper expression list");

  args = curr_pair->cdr;
  return curr_pair->car;
}

pair::operator std::string() const {
  std::string result("(" + to_string(car));

  sp<datum> p_tail(cdr);
  for (sp<pair> curr_pair(std::dynamic_pointer_cast<pair>(p_tail));
       curr_pair;) {
    p_tail = curr_pair->cdr;

    result += " " + to_string(curr_pair->car);
    curr_pair = std::dynamic_pointer_cast<pair>(p_tail);
  }
  if (p_tail) {
    result += " . " + to_string(p_tail);
  }

  return result + ")";
}

sp<datum> closure::call(sp<datum> args, const sp<env> &curr_env) const {
  const sp<env> eval_env(std::make_shared<env>(context));

  const auto eval_body([&]() -> sp<datum> {
    sp<datum> result;
    sp<pair> exprs(body);
    while (exprs) {
      result = eval(exprs->car, eval_env);
      exprs = std::dynamic_pointer_cast<pair>(exprs->cdr);
    }
    return result;
  });

  for (size_t i(0); i + 1 < formals.size(); ++i) {
    eval_env->define(formals[i], eval_next(args, curr_env));
  }

  if (!variadic) {
    if (!formals.empty()) {
      eval_env->define(formals.back(), eval_next(args, curr_env));
    }
    assert(!args && "too many args");

    return eval_body();
  }

  if (!args) {
    eval_env->define(formals.back(), nullptr);
    return eval_body();
  }

  sp<pair> tail(std::make_shared<pair>(eval_next(args, curr_env)));
  eval_env->define(formals.back(), tail);
  while (args) {
    sp<pair> new_tail(std::make_shared<pair>(eval_next(args, curr_env)));
    tail->cdr = new_tail;
    tail = new_tail;
  }
  return eval_body();
}
