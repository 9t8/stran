#ifndef _stranx_object_h_
#define _stranx_object_h_

#include <string>

struct object {
	friend std::ostream &operator<<(std::ostream &os, const object &t) {
		return os << static_cast<std::string>(t);
	}

	virtual ~object() {}

	virtual operator std::string() const = 0;
};

#endif
