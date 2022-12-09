#include "filtered_input.h"

#include <cassert>

int filtered_input::get() {
	int c(is.get());

	switch (c) {
		case '\n':
			col = 0;
			++row;
		case EOF:
			return c;
	}

	if (std::isprint(c) || c == '\t') {
		++col;
		return c;
	}

	std::cerr << "ERROR - unexpected character: character code " << c << "\n";
	throw;
}
