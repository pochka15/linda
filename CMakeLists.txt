cmake_minimum_required(VERSION 3.21)
project(linda)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost
        1.78.0 REQUIRED
        COMPONENTS thread)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(agent src/Agent_lib/main.cpp)
target_link_libraries(agent PUBLIC Agent_lib Common_lib Coordinator_lib Boost::thread)

add_executable(coordinator src/Coordinator_lib/main.cpp)
target_link_libraries(coordinator PUBLIC Coordinator_lib Boost::thread)

add_subdirectory(src/Agent_lib)
add_subdirectory(src/Common_lib)
add_subdirectory(src/Coordinator_lib)
add_subdirectory(src/Doctest_tests)