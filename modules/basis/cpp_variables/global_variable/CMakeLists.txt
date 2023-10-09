project(cpp_variables_global_variable CXX)

add_library(${PROJECT_NAME} SHARED # segement fault when using STATIC
    global_variables.cpp
)

add_example(test_global_variables.cpp
    ${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
)

add_example(test_access_global_variable.cpp
    ${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
)
