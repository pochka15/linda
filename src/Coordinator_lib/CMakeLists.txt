add_library(Coordinator_lib
        LindaCoordinator.cpp LindaCoordinator.h)
target_include_directories(Coordinator_lib PUBLIC
        "${PROJECT_SOURCE_DIR}/src/Common_lib")
