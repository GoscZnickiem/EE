#ifndef EE_LIB_UTIL_VISITOR
#define EE_LIB_UTIL_VISITOR

namespace ee {

template <typename... Callable>
struct Visitor : Callable... {
	using Callable::operator()...;
};

} // namespace ee

#endif // !EE_LIB_UTIL_VISITOR
