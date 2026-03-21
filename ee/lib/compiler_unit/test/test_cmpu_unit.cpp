#include <compiler_unit/Unit.hpp>
#include <gtest/gtest.h>
#include <ranges>
#include <string>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

namespace test {

class Unit : public ee::cmpu::Unit<int, bool> {
public:
	emitted_type emit() override {
		return (get() % 2) == 0;
	}

	[[nodiscard]] bool done() const override {
		return input_empty();
	}
};

class Unit2 : public ee::cmpu::Unit<bool, std::string> {
public:
	emitted_type emit() override {
		return get() ? "even" : "odd";
	}

	[[nodiscard]] bool done() const override {
		return input_empty();
	}
};

class Emitter : public ee::cmpu::Emitter<int> {
private:
	std::vector<int> data;
	std::size_t index{};

public:
	explicit Emitter(std::vector<int> input) : data(std::move(input)) {}

	emitted_type emit() override {
		return data.at(index++);
	}

	[[nodiscard]] bool done() const override {
		return index >= data.size();
	}
};

class Receiver : public ee::cmpu::Receiver<std::string> {
private:
	std::vector<std::string> data;

public:
	[[nodiscard]] std::vector<std::string> get_data() {
		data.clear();
		while (!input_empty()) data.push_back(get());
		return data;
	}
};

} // namespace test

TEST(cmpu_Unit, TransformsInput) {
	std::vector<int> ref{4, 8, 15, 16, 23, 42};
	std::vector<bool> res;
	test::Emitter e(ref);
	test::Unit u;

	u << e;

	while (u) {
		bool n = u.emit();
		res.push_back(n);
	}

	auto transformed = ref | std::views::transform([](int i) { return (i % 2) == 0; }) | std::ranges::to<std::vector>();

	EXPECT_EQ(res, transformed);
}

TEST(cmpu_Unit, Chains) {
	std::vector<int> ref{4, 8, 15, 16, 23, 42};
	test::Emitter e(ref);
	test::Receiver r;
	test::Unit u1;
	test::Unit2 u2;

	r << u2 << u1 << e;

	std::vector<std::string> res = r.get_data();

	auto transformed = ref | std::views::transform([](int x) { return (x % 2 == 0) ? std::string("even") : std::string("odd"); })
					 | std::ranges::to<std::vector>();

	EXPECT_EQ(res, transformed);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
