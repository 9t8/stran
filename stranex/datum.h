#ifndef _stranex_datum_h_
#define _stranex_datum_h_

#include "object.h"

#include <vector>
#include <unordered_map>

struct datum;

typedef std::unordered_map<std::string, std::shared_ptr<const datum>> env_type;

typedef std::vector<std::shared_ptr<const datum>> syntax_tree;

struct datum : object, std::enable_shared_from_this<datum> {
	virtual std::shared_ptr<const datum> eval(env_type &env) const {
		return shared_from_this();
	}
};

struct procedure : datum {
	virtual std::shared_ptr<const datum> call(
		env_type &env, std::vector<std::unique_ptr<const datum>> &args
	) const;

	std::vector<std::string> params;

	std::unique_ptr<const datum> body;
};

struct list : datum {
	operator std::string() const override;

	syntax_tree elements;
};

#endif
