#include "stream/Stream.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class test_int_stream : public ee::stream::Stream<int> {
	std::vector<int> data;
	size_t idx = 0;

public:
	explicit test_int_stream(std::vector<int> d) : data(std::move(d)) {}
	int get() override {
		return data[idx++];
	}
	[[nodiscard]] bool done() const override {
		return idx >= data.size();
	}
};

class test_ptr_stream : public ee::stream::Stream<std::unique_ptr<int>> {
	std::vector<std::unique_ptr<int>> data;
	size_t idx = 0;

public:
	explicit test_ptr_stream(std::vector<std::unique_ptr<int>> d) : data(std::move(d)) {}
	std::unique_ptr<int> get() override {
		return std::move(data[idx++]);
	}
	[[nodiscard]] bool done() const override {
		return idx >= data.size();
	}
};

TEST(test_stream, ProducesCorrectValues) {
	test_int_stream s({1, 2, 3});
	std::vector<int> result;

	while (!s()) {
		int x = 0;
		s >> x;
		result.push_back(x);
	}

	EXPECT_EQ(result, (std::vector<int>{1, 2, 3}));
}

TEST(test_stream, MovesValuesCorrectly) {
	std::vector<std::unique_ptr<int>> input;
	input.push_back(std::make_unique<int>(10));
	input.push_back(std::make_unique<int>(20));

	test_ptr_stream s(std::move(input));
	std::vector<int> result;

	while (!s()) {
		std::unique_ptr<int> p;
		s >> p;
		result.push_back(*p);
	}

	EXPECT_EQ(result, (std::vector<int>{10, 20}));
}

TEST(test_stream, OperatorCheckDone) {
	test_int_stream s({42});
	EXPECT_FALSE(s());
	int x = 0;
	s >> x;
	EXPECT_TRUE(s());
}

TEST(test_stream, OperatorShiftReturnsThis) {
	test_int_stream s({7});
	int x = 0;
	auto& r = (s >> x);
	EXPECT_EQ(&r, &s);
	EXPECT_EQ(x, 7);
}

TEST(test_stream, MoveOnlyStringStream) {
	class StringStream : public ee::stream::Stream<std::string> {
		std::vector<std::string> data;
		size_t idx = 0;

	public:
		explicit StringStream(std::vector<std::string> d) : data(std::move(d)) {}
		std::string get() override {
			return std::move(data[idx++]);
		}
		[[nodiscard]] bool done() const override {
			return idx >= data.size();
		}
	};

	StringStream s({"foo", "bar"});
	std::vector<std::string> result;

	while (!s()) {
		std::string str;
		s >> str;
		result.push_back(std::move(str));
	}

	EXPECT_EQ(result, (std::vector<std::string>{"foo", "bar"}));
}
