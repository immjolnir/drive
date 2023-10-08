project(basis_memory_debugger CXX)

set(FILES
    leak_example.cpp
    uninitialized_variable.cpp
)

foreach(f ${FILES})
    add_example(${f} ${GTEST_BOTH_LIBRARIES})
endforeach()


