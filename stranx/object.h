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

typedef std::vector<std::shared_ptr<object>> token_list;

struct beginl : object {
	operator std::string() const override {
		return "(";
	}
};

struct endl : object {
	operator std::string() const override {
		return ")";
	}
};

struct dot : object {
	operator std::string() const override {
		return ".";
	}
};

#endif
