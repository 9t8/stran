#ifndef _stran_tok_h_
#define _stran_tok_h_

#include <string>
#include <vector>

namespace stran {

template <class t> using sp = std::shared_ptr<t>;

template <class t, class u> sp<t> sp_cast(const sp<u> &r) {
  t *p = dynamic_cast<t *>(r.get());
  return p ? sp<t>(r, p) : sp<t>();
}

template <class t, class... types> sp<t> make_sp(types &&...args) {
  return sp<t>(new t(std::forward<types>(args)...));
}

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

} // namespace stran

#endif
