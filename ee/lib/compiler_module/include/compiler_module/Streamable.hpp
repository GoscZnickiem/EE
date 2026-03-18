#ifndef EE_LIB_COMPILERMODULE_STREAMABLE
#define EE_LIB_COMPILERMODULE_STREAMABLE

#include <concepts>

namespace ee::cmod {

template <typename T>
concept Streamable = std::movable<T> || std::copyable<T>;

}

#endif // !EE_LIB_COMPILERMODULE_STREAMABLE
