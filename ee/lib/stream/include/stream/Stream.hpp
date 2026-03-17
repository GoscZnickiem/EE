#ifndef EE_LIB_STREAM_STREAM
#define EE_LIB_STREAM_STREAM

#include <concepts>

namespace ee::stream {

template <typename T>
concept Streamable = std::movable<T> || std::copyable<T>;

template <Streamable T>
class Stream {
public:
	Stream() = default;
	virtual ~Stream() = default;
	Stream(const Stream&) = delete;
	Stream(Stream&&) = delete;
	Stream& operator=(const Stream&) = delete;
	Stream& operator=(Stream&&) = delete;

	virtual T get() = 0;
	[[nodiscard]] virtual bool done() const = 0;

	Stream<T>& operator>>(T& output) {
		output = get();
		return *this;
	}

	bool operator()() const {
		return done();
	}
};

} // namespace ee::stream

#endif // !EE_LIB_STREAM_STREAM
