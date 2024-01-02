project(house_sparrow CXX)

set(ALL_FILES
    tax.cpp

    # multithreading: worker vs pool?
    # test_thread_worker.cpp
    # test_thread_pool.cpp
)

foreach(f ${ALL_FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()
