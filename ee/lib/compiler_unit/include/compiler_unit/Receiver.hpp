#ifndef EE_LIB_COMPILERUNIT_RECEIVER
#define EE_LIB_COMPILERUNIT_RECEIVER

#include "compiler_unit/Emitter.hpp"
#include "compiler_unit/Streamable.hpp"

#include <concepts>
#include <stdexcept>

namespace ee::cmpu {

template <Streamable ReceivedType>
class Receiver {
public:
	using received_type = ReceivedType;

	Receiver() = default;
	virtual ~Receiver() = default;
	Receiver(const Receiver&) = delete;
	Receiver(Receiver&&) = delete;
	Receiver& operator=(const Receiver&) = delete;
	Receiver& operator=(Receiver&&) = delete;

	[[nodiscard]] bool connected() const noexcept {
		return provider;
	}

	[[nodiscard]] bool input_empty() const noexcept {
		return provider ? provider->done() : true;
	}

	template <Emitter_of_c<received_type> E>
	E& operator<<(E& input) noexcept {
		provider = &input;
		return input;
	}

protected:
	ReceivedType get() const {
		if (input_empty()) throw std::runtime_error{"Input to the receiver is empty"};
		return provider->emit();
	}

private:
	Emitter<ReceivedType>* provider{nullptr};
};

template <typename T>
concept Receiver_c = requires { typename T::received_type; } && std::derived_from<T, Receiver<typename T::received_type>>;

template <typename T, typename ReceivedType>
concept Receiver_of_c = Receiver_c<T> && std::same_as<typename T::received_type, ReceivedType>;

} // namespace ee::cmpu

#endif // !EE_LIB_COMPILERUNIT_RECEIVER
