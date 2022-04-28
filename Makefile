prepare:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -B cmake-build-debug

build:
	cmake --build cmake-build-debug --target linda Doctest_tests_run

run:
	./cmake-build-debug/linda

test:
	./cmake-build-debug/src/Doctest_tests/Doctest_tests_run