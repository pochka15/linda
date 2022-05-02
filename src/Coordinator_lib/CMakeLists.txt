add_library(Coordinator_lib
        LindaCoordinator.cpp LindaCoordinator.h main.cpp)

target_include_directories(Coordinator_lib PUBLIC
        "${PROJECT_SOURCE_DIR}/src/Common_lib")

add_executable(coordinator main.cpp)
target_link_libraries(coordinator PUBLIC Common_lib Coordinator_lib)
