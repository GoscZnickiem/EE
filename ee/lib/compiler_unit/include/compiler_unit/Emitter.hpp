#ifndef EE_LIB_COMPILERUNIT_EMITTER
#define EE_LIB_COMPILERUNIT_EMITTER

#include "compiler_unit/Streamable.hpp"

#include <concepts>

namespace ee::cmpu {

template <Streamable EmittedType>
class Emitter {
public:
	using emitted_type = EmittedType;

	Emitter() = default;
	virtual ~Emitter() = default;
	Emitter(const Emitter&) = delete;
	Emitter(Emitter&&) = delete;
	Emitter& operator=(const Emitter&) = delete;
	Emitter& operator=(Emitter&&) = delete;

	virtual emitted_type emit() = 0;
	[[nodiscard]] virtual bool done() = 0;

	explicit operator bool() {
		return !done();
	}
};

template <typename T>
concept Emitter_c = requires { typename T::emitted_type; } && std::derived_from<T, Emitter<typename T::emitted_type>>;

template <typename T, typename EmittedType>
concept Emitter_of_c = Emitter_c<T> && std::same_as<typename T::emitted_type, EmittedType>;

template <Emitter_c T>
T& operator>>(T& emitter, typename T::emitted_type& output) {
	output = emitter.emit();
	return emitter;
}

} // namespace ee::cmpu

#endif // !EE_LIB_COMPILERUNIT_EMITTER
