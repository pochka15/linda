add_library(Agent_lib LindaAgent.cpp LindaAgent.h)
target_link_libraries(Agent_lib PUBLIC Common_lib)
target_include_directories(Agent_lib PUBLIC "${PROJECT_SOURCE_DIR}/src/Common_lib")

