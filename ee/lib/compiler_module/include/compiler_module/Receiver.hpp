#ifndef EE_LIB_COMPILERMODULE_RECEIVER
#define EE_LIB_COMPILERMODULE_RECEIVER

#include "compiler_module/Emitter.hpp"
#include "compiler_module/Streamable.hpp"

#include <stdexcept>

namespace ee::cmod {

template <Streamable ReceivedType>
class Receiver {
public:
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

	ReceivedType get() const {
		if (input_empty()) throw std::runtime_error{"Input to the receiver is empty"};
		return provider->emit();
	}

	template <Emitter_c Emiter>
	Emiter& operator<<(Emiter& input) noexcept {
		provider = &input;
		return input;
	}

private:
	Emitter<ReceivedType>* provider;
};

} // namespace ee::cmod

#endif // !EE_LIB_COMPILERMODULE_RECEIVER
