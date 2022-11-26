#ifndef _stranx_filtered_input_h_
#define _stranx_filtered_input_h_

#include <iostream>

struct filtered_input {
	filtered_input(std::istream &i) : is(i), row(1), col(0) {}

	int get();

private:
	std::istream &is;

	size_t row;

	size_t col;
};

#endif
