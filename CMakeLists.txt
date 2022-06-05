cmake_minimum_required(VERSION 3.21)
project(linda)

set(CMAKE_CXX_STANDARD 17)

add_executable(agent src/Agent_lib/main.cpp)
target_link_libraries(agent PUBLIC Agent_lib Common_lib Coordinator_lib)

add_executable(coordinator src/Coordinator_lib/main.cpp)
target_link_libraries(coordinator PUBLIC Coordinator_lib)

add_subdirectory(src/Agent_lib)
add_subdirectory(src/Common_lib)
add_subdirectory(src/Coordinator_lib)
add_subdirectory(src/Doctest_tests)