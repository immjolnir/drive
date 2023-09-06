project(mygtest)

add_library(${PROJECT_NAME} STATIC
    edit_distance.cpp
    floating_point_number_helper.cpp
)

set(ALL_FILES
    test_floating_point_number_helper.cpp
)

# On the basis of Gtest, creating the cases.
foreach(file ${ALL_FILES})
    add_example(${file} 
        ${PROJECT_NAME}
        ${GTEST_BOTH_LIBRARIES}
    )
endforeach()