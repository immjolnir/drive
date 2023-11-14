project(algos CXX)

add_header()

set(TEST_FILES
    kdtree/kdtree_test.cpp
    disjoint_set/test_uf.cpp
)

set(KDTREE_FILES
    kdtree/hdu2966.cpp
)

set(ALL_FILES
    ${TEST_FILES}
    ${KDTREE_FILES}
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()

add_footer()