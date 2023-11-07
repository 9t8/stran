#include "parse.h"

#include "datum.h"

#include <vector>

using namespace stran;

tok_list stran::lex(const std::string &s) {
  tok_list toks;

  for (size_t i(0), word_start(0);;) {
    if (i < s.size()) {
      switch (s[i]) {
      case ' ':
      case '\n':
      case '\t':
      case '(':
      case ')':
      case ';':
      case '\'':
        break;

      default:
        ++i;
        continue;
      }
    }

    if (i - word_start > 0) {
      std::string word(s.substr(word_start, i - word_start));
      if (word == ".") {
        toks.push_back(make_sp<dot>());
      } else if (word == "+" || word == "-" ||
                 (word[0] != '+' && word[0] != '-' && word[0] != '.' &&
                  (word[0] < '0' || word[0] > '9'))) {
        toks.push_back(make_sp<iden>(word));
      } else {
        size_t idx(0);
        double val(stod(word, &idx));
        assert(idx == word.size() && "invalid character while parsing number");
        toks.push_back(make_sp<inexact>(val));
      }
    }

    if (i >= s.size()) {
      return toks;
    }

    switch (s[i]) {
    case '(':
      toks.push_back(make_sp<beginl>());
      break;

    case ')':
      toks.push_back(make_sp<endl>());
      break;

    case ';':
      while (s[i] != '\n' && i < s.size()) {
        ++i;
      }
      break;

    case '\'':
      toks.push_back(make_sp<apos>());
      break;
    }
    word_start = ++i;
  }
}

sp<datum> stran::parse_datum(const tok_list &toks, size_t &i) {
  const auto peek_next_type([&]() -> const std::type_info & {
    assert(i < toks.size() && "expected more tokens but none found");

    tok &next_tok(*toks[i]);
    return typeid(next_tok);
  });

  if (peek_next_type() == typeid(beginl)) {
    ++i;
    if (peek_next_type() == typeid(endl)) {
      ++i;
      return nullptr;
    }

    sp<pair> p(make_sp<pair>(parse_datum(toks, i)));
    sp<datum> start(p);
    while (peek_next_type() != typeid(endl)) {
      if (peek_next_type() == typeid(dot)) {
        p->cdr = parse_datum(toks, ++i);
        assert(peek_next_type() == typeid(endl) && "misplaced dot token");
        break;
      }
      sp<pair> p_new(make_sp<pair>(parse_datum(toks, i)));
      p->cdr = p_new;
      p = p_new;
    }
    ++i;
    return start;
  }
  if (peek_next_type() == typeid(apos)) {
    return make_sp<pair>(make_sp<iden>("quote"),
                         make_sp<pair>(parse_datum(toks, ++i)));
  }

  sp<datum> p(sp_cast<datum>(toks.at(i++)));
  assert(p && "tried to parse an unparsable token");

  return p;
}
