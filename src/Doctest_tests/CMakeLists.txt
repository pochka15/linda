add_executable(Doctest_tests_run LindaTupleUtilsTest.cpp)
target_link_libraries(Doctest_tests_run PUBLIC Linda_lib)
target_include_directories(Doctest_tests_run PUBLIC "${PROJECT_SOURCE_DIR}/src/Linda_lib")

