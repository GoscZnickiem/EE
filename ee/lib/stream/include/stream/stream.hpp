#ifndef EE_LIB_STREAM_STREAM
#define EE_LIB_STREAM_STREAM

#include <concepts>

namespace ee::stream {

template <typename T>
concept Streamable = std::movable<T> || std::copyable<T>;

template <Streamable T>
class stream {
public:
	stream() = default;
	virtual ~stream() = default;
	stream(const stream&) = delete;
	stream(stream&&) = delete;
	stream& operator=(const stream&) = delete;
	stream& operator=(stream&&) = delete;

	virtual T get() = 0;
	[[nodiscard]] virtual bool done() const = 0;

	stream<T>& operator>>(T& output) {
		output = get();
		return *this;
	}

	bool operator()() const {
		return done();
	}
};

} // namespace ee::stream

#endif // !EE_LIB_STREAM_STREAM
