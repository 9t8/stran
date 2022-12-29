#ifndef _stranx_token_h_
#define _stranx_token_h_

#include <string>

namespace stranx {
	
	template <typename t> using sp = std::shared_ptr<t>;

	struct token {
		friend std::ostream &operator<<(std::ostream &os, const token &t) {
			return os << static_cast<std::string>(t);
		}

		virtual ~token() {}

		virtual operator std::string() const = 0;
	};

	typedef std::vector<sp<token>> token_list;

	struct beginl : token {
		operator std::string() const override {
			return "(";
		}
	};

	struct endl : token {
		operator std::string() const override {
			return ")";
		}
	};

	struct dot : token {
		operator std::string() const override {
			return ".";
		}
	};
	
	struct quote_tok : token {
		operator std::string() const override {
			return "'";
		}
	};

}

#endif
