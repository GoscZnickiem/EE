#include <compiler_unit/Emitter.hpp>
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

namespace test {

class Emitter : public ee::cmpu::Emitter<std::unique_ptr<int>> {
private:
	std::vector<int> data;
	std::size_t index{};

public:
	explicit Emitter(std::vector<int> input) : data(std::move(input)) {}

	emitted_type emit() override {
		return std::make_unique<int>(data.at(index++));
	}

	[[nodiscard]] bool done() override {
		return index >= data.size();
	}
};

class Emitter2 : public ee::cmpu::Emitter<int> {
private:
	std::vector<int> data;
	std::size_t index{};

public:
	explicit Emitter2(std::vector<int> input) : data(std::move(input)) {}

	emitted_type emit() override {
		return data.at(index++);
	}

	[[nodiscard]] bool done() override {
		return index >= data.size();
	}
};

} // namespace test

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

TEST(cmpu_Emitter, EmitsCorrectData) {
	std::vector<int> ref{4, 8, 15, 16, 23, 42};
	std::vector<int> res;
	std::vector<int> res2;
	test::Emitter e(ref);
	test::Emitter e2(ref);

	while (e) {
		std::unique_ptr<int> n;
		e >> n;
		res.push_back(*n);
	}

	while (!e2.done()) {
		std::unique_ptr<int> n;
		n = e2.emit();
		res2.push_back(*n);
	}

	EXPECT_EQ(res, ref);
	EXPECT_EQ(res2, ref);
}

TEST(cmpu_Emmiter, StreamOutOperatorReturnsEmitter) {
	test::Emitter2 e({1});
	int a = 0;

	EXPECT_EQ(&(e >> a), &e);
}

TEST(cmpu_Emmiter, EmitOperatorChains) {
	std::vector<int> ref{1, 2, 3};
	test::Emitter2 e(ref);
	int a = 0;
	int b = 0;
	int c = 0;

	e >> a >> b >> c;

	EXPECT_EQ(a, 1);
	EXPECT_EQ(b, 2);
	EXPECT_EQ(c, 3);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
