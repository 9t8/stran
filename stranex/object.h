#ifndef _stranex_object_h_
#define _stranex_object_h_

#include <string>

struct object {
	virtual ~object() {}

	virtual operator std::string() const = 0;

	friend std::ostream &operator<<(std::ostream &os, const object &t) {
		return os << static_cast<std::string>(t);
	}
};

#endif
