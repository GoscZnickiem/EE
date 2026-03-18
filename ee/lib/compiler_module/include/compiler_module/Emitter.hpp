#ifndef EE_LIB_COMPILERMODULE_EMITTER
#define EE_LIB_COMPILERMODULE_EMITTER

#include "compiler_module/Streamable.hpp"

#include <concepts>

namespace ee::cmod {

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
	[[nodiscard]] virtual bool done() const = 0;

	Emitter<emitted_type>& operator>>(emitted_type& output) {
		output = emit();
		return *this;
	}

	bool operator()() const {
		return done();
	}
};

template <typename T>
concept Emitter_c = requires { typename T::emitted_type; } && std::derived_from<T, Emitter<typename T::emitted_type>>;

} // namespace ee::cmod

#endif // !EE_LIB_COMPILERMODULE_EMITTER
