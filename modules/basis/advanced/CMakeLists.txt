project(basis_advanced C)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread") 

set(ALL_FILES
    memory_protection.c
    pthreads_sched_test.c
)

foreach(file ${ALL_FILES})
    add_example(${file}
        # ${GTEST_LIBRARIES}
        atomic # /usr/include/c++/11/atomic:361: error: undefined reference to '__atomic_compare_exchange_16'
    )
endforeach()

