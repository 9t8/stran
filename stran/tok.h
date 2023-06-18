#ifndef _stran_tok_h_
#define _stran_tok_h_

#include <string>

namespace stran {

template <typename t>
using sp = std::shared_ptr<t>;

struct tok {
  friend std::string to_string(const sp<tok> &p_t) {
    return p_t ? static_cast<std::string>(*p_t) : "()";
  }

  virtual ~tok() {}

 private:
  virtual operator std::string() const = 0;
};

typedef std::vector<sp<tok>> tok_list;

struct beginl : tok {
 private:
  operator std::string() const override { return "("; }
};

struct endl : tok {
 private:
  operator std::string() const override { return ")"; }
};

struct dot : tok {
 private:
  operator std::string() const override { return "."; }
};

struct apos : tok {
 private:
  operator std::string() const override { return "'"; }
};

}  // namespace stran

#endif
