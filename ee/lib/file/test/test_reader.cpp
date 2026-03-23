#include <cstddef>
#include <file/Reader.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <types/Character.hpp>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

TEST(file_Reader, ReadsCharacters) {
	ee::file::Reader reader;
	test::Emitter e({});

	EXPECT_FALSE(r.connected());
	r << e;
	EXPECT_TRUE(r.connected());
}

TEST(cmpu_Receiver, ReceivesDataFromEmitter) {
	std::vector<int> ref{4, 8, 15, 16, 23, 42};
	test::Receiver r;
	test::Emitter e(ref);

	EXPECT_TRUE(r.get_data().empty());
	r << e;
	EXPECT_EQ(r.get_data(), ref);
}

TEST(cmpu_Receiver, ThrowsWhenInputIsEmpty) {
	test::Receiver r;
	EXPECT_THROW(r.public_get(), std::runtime_error);
}

TEST(cmpu_Receiver, StreamInOperatorReturnsEmitter) {
	test::Receiver r;
	test::Emitter e({});

	EXPECT_EQ(&(r << e), &e);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
