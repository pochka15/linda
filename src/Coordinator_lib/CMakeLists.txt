add_library(Coordinator_lib
        LindaCoordinator.cpp LindaCoordinator.h main.cpp)

target_link_libraries(Coordinator_lib PUBLIC Common_lib)

target_include_directories(Coordinator_lib PUBLIC
        "${PROJECT_SOURCE_DIR}/src/Common_lib")
