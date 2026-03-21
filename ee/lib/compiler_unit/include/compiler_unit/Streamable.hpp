#ifndef EE_LIB_COMPILERUNIT_STREAMABLE
#define EE_LIB_COMPILERUNIT_STREAMABLE

#include <concepts>

namespace ee::cmpu {

template <typename T>
concept Streamable = std::movable<T> || std::copyable<T>;

}

#endif // !EE_LIB_COMPILERUNIT_STREAMABLE
