#include <iostream>

#include "datum.h"

namespace stran {

const sp<datum> &env::find(const std::string &name) const {
  const decltype(table)::const_iterator it(table.find(name));
  if (it != table.end()) {
    return it->second;
  }
  if (!parent) {
    std::cerr << "ERROR - undefined identifier: " << name << "\n";
    assert(0);
  }
  return parent->find(name);
}

}  // namespace stran
