scenario = --scenario /Users/pochka15/projects/c++/linda/scenarios
reader = ./cmake-build-debug/agent --reader
writer = ./cmake-build-debug/agent --writer
terminator = (./cmake-build-debug/agent --reader --scenario /Users/pochka15/projects/c++/linda/scenarios/terminator.json &)

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

run_terminator:
	$(terminator)

test:
	./cmake-build-debug/src/Doctest_tests/Doctest_tests_run

# -> Writer is waiting for requests
scenario_1:
	($(reader) &) && \
	$(writer)

# Unblock writer using VIP reader -> All processes terminated
run_hello_vip_reader:
	($(reader) $(scenario)/helloVipReader.json)

run_hello_writer:
	($(writer) $(scenario)/helloWriter.json)

# -> Reader and writer have terminated
scenario_3:
	(($(writer) $(scenario)/helloWriter.json &) && \
	($(reader) $(scenario)/helloVipReader.json))

# Four readers
scenario_4:
	($(reader) &) && \
	($(reader) &) && \
	($(reader) &) && \
	($(reader) &) && \
	$(writer)

# Different readers and writers
scenario_5:
	($(reader) $(scenario)/byeReader.json &) && \
	($(reader) &) && \
	($(reader) $(scenario)/byeReader.json &) && \
	($(reader) &) && \
	($(writer) &) && \
	$(writer) $(scenario)/byeWriter.json

scenario_6:
	$(reader) $(scenario)/byeReaderVip.json