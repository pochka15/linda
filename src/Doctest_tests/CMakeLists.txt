add_executable(Doctest_tests_run
        LindaTupleUtilsTest.cpp
        CommunicationServiceTest.cpp
        RandomUtilsTest.cpp)
target_link_libraries(Doctest_tests_run PUBLIC Common_lib Coordinator_lib Agent_lib)
target_include_directories(Doctest_tests_run PUBLIC
        "${PROJECT_SOURCE_DIR}/src/Common_lib"
        "${PROJECT_SOURCE_DIR}/src/Coordinator_lib"
        "${PROJECT_SOURCE_DIR}/src/Agent_lib")

