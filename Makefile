prepare:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -B cmake-build-debug

build:
	cmake --build cmake-build-debug --target agents Doctest_tests_run coordinator

run_agents:
	./cmake-build-debug/agents

run_coordinator:
	./cmake-build-debug/coordinator &

test:
	./cmake-build-debug/src/Doctest_tests/Doctest_tests_run
