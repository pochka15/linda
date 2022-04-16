cmake_minimum_required(VERSION 3.21)
project(linda)

set(CMAKE_CXX_STANDARD 17)

add_executable(linda src/main.cpp)
target_link_libraries(linda PUBLIC Linda_lib)

add_subdirectory(src/Linda_lib)
add_subdirectory(src/Doctest_tests)

target_include_directories(linda PUBLIC "${PROJECT_SOURCE_DIR}/src/Linda_lib")
