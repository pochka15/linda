prepare:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -B cmake-build-debug

build:
	cmake --build cmake-build-debug --target linda

run:
	./cmake-build-debug/linda
