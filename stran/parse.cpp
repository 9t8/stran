#include <istream>
#include <vector>

#include "datum.h"
#include "parse.h"

namespace stran {

tok_list lex(std::istream &is) {
  tok_list toks;

  std::string curr_word;
  for (;;) {
    int curr_char(is.get());

    switch (curr_char) {
      case ' ':
      case '\n':
      case '\t':
      case EOF:
      case '(':
      case ')':
      case ';':
      case '\'':
        break;

      default:
        curr_word += static_cast<char>(curr_char);
        continue;
    }

    if (!curr_word.empty()) {
      if (curr_word == ".") {
        toks.push_back(std::make_shared<dot>());
      } else if ((curr_word[0] != '+' && curr_word[0] != '-' &&
                  curr_word[0] != '.' &&
                  (curr_word[0] < '0' || curr_word[0] > '9')) ||
                 curr_word == "+" || curr_word == "-") {
        toks.push_back(std::make_shared<iden>(curr_word));
      } else {
        size_t idx(0);
        double val(stod(curr_word, &idx));
        assert(idx == curr_word.size() &&
               "invalid character while parsing number");
        toks.push_back(std::make_shared<inexact>(val));
      }
      curr_word.clear();
    }

    switch (curr_char) {
      case EOF:
        return toks;

      case '(':
        toks.push_back(std::make_shared<beginl>());
        break;

      case ')':
        toks.push_back(std::make_shared<endl>());
        break;

      case ';':
        while (curr_char != '\n' && curr_char != EOF) {
          curr_char = is.get();
        }
        break;

      case '\'':
        toks.push_back(std::make_shared<apos>());
        break;
    }
  }
}

sp<datum> parse_datum(const tok_list &toks, size_t &idx) {
  const auto peek_next_type([&]() -> const std::type_info & {
    assert(idx < toks.size() && "expected more tokens but none found");

    tok &next_tok(*toks[idx]);
    return typeid(next_tok);
  });

  if (peek_next_type() == typeid(beginl)) {
    ++idx;
    if (peek_next_type() == typeid(endl)) {
      ++idx;
      return nullptr;
    }

    sp<pair> p(std::make_shared<pair>(parse_datum(toks, idx)));
    sp<datum> start(p);
    while (peek_next_type() != typeid(endl)) {
      if (peek_next_type() == typeid(dot)) {
        p->cdr = parse_datum(toks, ++idx);
        assert(peek_next_type() == typeid(endl) && "misplaced dot token");
        break;
      }
      sp<pair> p_new(std::make_shared<pair>(parse_datum(toks, idx)));
      p->cdr = p_new;
      p = p_new;
    }
    ++idx;
    return start;
  }
  if (peek_next_type() == typeid(apos)) {
    return std::make_shared<pair>(
        std::make_shared<iden>("quote"),
        std::make_shared<pair>(parse_datum(toks, ++idx)));
  }

  sp<datum> p(sp_cast<datum>(toks.at(idx++)));
  assert(p && "tried to parse an unparsable token");

  return p;
}

}  // namespace stran
