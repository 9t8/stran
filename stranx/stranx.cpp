#include "closure.h"
#include "lexer.h"

#include <iostream>

using namespace stranx;

sp<datum> parse_next(const tok_list &toks, size_t &idx) {
	const auto get_next_tok_type([&]() -> const std::type_info & {
		assert(idx < toks.size() &&
		"expected a token but none found (too many opening parens?)");

		tok &next_tok(*toks[idx]);
		return typeid(next_tok);
	});

	if (get_next_tok_type() == typeid(beginl)) {
		++idx;
		if (get_next_tok_type() == typeid(endl)) {
			++idx;
			return std::make_shared<emptyl>();
		}

		sp<pair> p(std::make_shared<pair>(parse_next(toks, idx)));
		sp<datum> start(p);
		while (get_next_tok_type() != typeid(endl)) {
			if (get_next_tok_type() == typeid(dot)) {
				p->cdr = parse_next(toks, ++idx);
				assert(get_next_tok_type() == typeid(endl) &&
					   "malformed improper list (misplaced dot tok)");
				break;
			}
			sp<pair> p_new(std::make_shared<pair>(parse_next(toks, idx)));
			p->cdr = p_new;
			p = p_new;
		}
		++idx;

		return start;
	}

	sp<datum> p(std::dynamic_pointer_cast<datum>(toks.at(idx++)));
	assert(p && "malformed token list (tried to parse an unparsable tok)");

	return p;
}

int main(int, const char *[]) {
	tok_list toks(lexer(std::cin).lex());

	std::cout << "===-- toks --===\n";
	for (size_t i(0); i < toks.size(); ++i) {
		std::cout << " " << *toks[i];
	}
	std::cout << "\n";

	std::vector<sp<datum>> tree;
	for (size_t i(0); i < toks.size();) {
		tree.push_back(parse_next(toks, i));
	}

	std::cout << "\n===-- tree --===\n";
	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << *tree[i] << "\n";
	}
	std::cout << "\n===-- output --===" << std::endl;

	const sp<env> top_level(std::make_shared<env>(nullptr));
	top_level->define("lambda", std::make_shared<lambda>());
	top_level->define("define", std::make_shared<define>());

	for (size_t i(0); i < tree.size(); ++i) {
		std::cout << *tree[i]->eval(top_level) << std::endl;
	}
}
