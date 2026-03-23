#include <compiler_unit/Receiver.hpp>
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

namespace test {

class Receiver : public ee::cmpu::Receiver<std::unique_ptr<int>> {
private:
	std::vector<int> data;

public:
	[[nodiscard]] std::vector<int> get_data() {
		data.clear();
		while (!input_empty()) {
			data.push_back(*get());
		}
		return data;
	}

	received_type public_get() {
		return get();
	}
};

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

} // namespace test

TEST(cmpu_Receiver, ConnectsToEmitter) {
	test::Receiver r;
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
