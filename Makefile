prepare:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -B cmake-build-debug

build:
	cmake --build cmake-build-debug --target agent Doctest_tests_run coordinator

run_writer:
	./cmake-build-debug/agent --writer

run_reader:
	./cmake-build-debug/agent --reader

run_coordinator:
	./cmake-build-debug/coordinator

test:
	./cmake-build-debug/src/Doctest_tests/Doctest_tests_run
