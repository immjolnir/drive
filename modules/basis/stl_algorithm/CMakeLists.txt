project(basis_stl_algorithm CXX)

set(SEARCHING
    min_element.cpp
    upper_lower_bounds.cpp
)

set(SORTING
    nth_element.cpp
)

set(PERMUTATION
    pre_permutation.cpp
    next_permutation.cpp
)

set(ALL_FILES
    ${SEARCHING}
    ${SORTING}
    ${PERMUTATION}
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()


