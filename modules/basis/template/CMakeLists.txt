project(basis_template CXX)

add_library(${PROJECT_NAME}
    extern_template.cpp
)

set(ALL_FILES
    test_extern_template.cpp
    test_compile_branch_template.cpp
    # test_partial_specialization.cpp
)

foreach(f ${ALL_FILES})
    add_example(${f} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
endforeach()


