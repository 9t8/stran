#include "parse.h"

#include <iostream>

namespace stran {

static sp<datum> lambda(sp<datum> &args, const sp<env> &curr_env) {
  std::vector<std::string> formals;

  sp<datum> p_formal(next(args));
  while (p_formal) {
    datum &formal(*p_formal);
    if (typeid(formal) == typeid(iden)) {
      formals.push_back(dynamic_cast<const iden &>(formal).name);
      return make_sp<closure>(formals, true, sp_cast<pair>(args), curr_env);
    }

    const sp<datum> p_next_formal(next(p_formal));
    assert(p_next_formal && "formals must not be nil");

    const datum &next_formal(*p_next_formal);
    assert(typeid(next_formal) == typeid(iden) &&
           "all formals must be identifiers");

    formals.push_back(dynamic_cast<const iden &>(next_formal).name);
  }

  return make_sp<closure>(formals, false, sp_cast<pair>(args), curr_env);
}

static sp<datum> define(sp<datum> &args, const sp<env> &curr_env) {
  sp<datum> p_car(next(args));
  assert(p_car && "first arg of define must not be nil");

  const datum &car(*p_car);
  if (typeid(car) == typeid(iden)) {
    curr_env->define(dynamic_cast<const iden &>(car).name,
                     eval_next(args, curr_env));
    assert(!args && "arg list too long or malformed");

    return nullptr;
  }

  assert(typeid(car) == typeid(pair) && "first arg must be identifier or list");

  const sp<datum> p_caar(next(p_car));
  assert(p_caar && "procedure name must not be nil");
  const datum &caar(*p_caar);
  assert(typeid(caar) == typeid(iden) &&
         "procedure name must be an identifier");

  sp<datum> p_formals(make_sp<pair>(p_car, args));
  curr_env->define(dynamic_cast<const iden &>(caar).name,
                   lambda(p_formals, curr_env));

  return nullptr;
}

static sp<datum> quote(sp<datum> &args, const sp<env> &) {
  const sp<datum> contents(next(args));
  assert(!args && "too many args");

  return contents;
}

} // namespace stran

int main(int, const char *[]) {
  stran::tok_list toks(stran::lex(std::cin));

  std::vector<stran::sp<stran::datum>> tree;
  for (size_t i(0); i < toks.size();) {
    tree.push_back(parse_datum(toks, i));
  }

  const stran::sp<stran::env> top_level(stran::make_sp<stran::env>(nullptr));
  top_level->define("lambda",
                    stran::make_sp<stran::native_func>(stran::lambda));
  top_level->define("define",
                    stran::make_sp<stran::native_func>(stran::define));
  top_level->define("quote", stran::make_sp<stran::native_func>(stran::quote));

  for (size_t i(0); i < tree.size(); ++i) {
    std::cout << to_string(eval(tree[i], top_level)) << std::endl;
  }
}
