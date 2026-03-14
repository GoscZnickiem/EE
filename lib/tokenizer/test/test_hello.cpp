#include <gtest/gtest.h>
#include "tokenizer/hello.hpp"

TEST(hello, world) {
	const auto r = hello();
	constexpr int exp = 42;

	EXPECT_TRUE(r == exp);
}
