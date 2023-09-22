project(cpp_variables_extern_vs_static CXX)

# shared vs static ??
add_library(${PROJECT_NAME} STATIC
    global.cpp
)

add_example(test_extern_vs_static.cpp
    ${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
)
