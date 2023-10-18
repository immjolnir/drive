project(basis_stl_algorithm CXX)

set(SORTING
    nth_element.cpp
)

set(PERMUTATION
    pre_permutation.cpp
    next_permutation.cpp
)

set(ALL_FILES
    ${SORTING}
    ${PERMUTATION}
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()


