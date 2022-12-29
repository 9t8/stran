#ifndef _stranx_tok_h_
#define _stranx_tok_h_

#include <string>

namespace stranx {
	
	template <typename t> using sp = std::shared_ptr<t>;

	struct tok {
		friend std::ostream &operator<<(std::ostream &os, const tok &t) {
			return os << static_cast<std::string>(t);
		}

		virtual ~tok() {}

		virtual operator std::string() const = 0;
	};

	typedef std::vector<sp<tok>> tok_list;

	struct beginl : tok {
		operator std::string() const override {
			return "(";
		}
	};

	struct endl : tok {
		operator std::string() const override {
			return ")";
		}
	};

	struct dot : tok {
		operator std::string() const override {
			return ".";
		}
	};
	
	struct quote_tok : tok {
		operator std::string() const override {
			return "'";
		}
	};

}

#endif
