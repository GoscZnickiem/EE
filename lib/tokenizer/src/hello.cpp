#include "tokenizer/hello.hpp"
#include <iostream>

int hello() {
	std::cout << ":3\n";

	constexpr auto ret = 42;
	return ret;
}
