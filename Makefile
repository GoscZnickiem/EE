.PHONY: all build test fresh

all: build

build:
	cmake --preset clang-debug
	cmake --build --preset clang-debug

test: build
	ctest --test-dir build/clang-debug

clean:
	rm -rf build/clang-debug

rebuild:
	cmake --preset clang-debug --fresh
	cmake --build --preset clang-debug

refresh:
	$(MAKE) clean
	$(MAKE) build
