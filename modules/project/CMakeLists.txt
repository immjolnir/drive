project(demo_project CXX)

message(STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")

add_library(${PROJECT_NAME} STATIC)

# Policy CMP0076 is not set: target_sources() command converts relative paths to absolute. 
# Run "cmake --help-policy CMP0076" for policy details. 
# Use the cmake_policy command to set the policy and suppress this warning.
set(SOURCES_DIR ${CATCH_DIR}/modules/project)

target_sources(${PROJECT_NAME}
    PUBLIC
        ${SOURCES_DIR}/employee.h
    PRIVATE
        ${SOURCES_DIR}/employee.cpp
)

target_include_directories(${PROJECT_NAME} BEFORE
    PUBLIC
        ${CMAKE_SOURCE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


# Adds unit tests, and if enabled, cmake project tests.
if (ARCHETYPE_BUILD_TESTING)
    message(STATUS "Adding tests under ${PROJECT_NAME}Tests...")

    set(TEST_SOURCES
        test_employee.cpp
        test_gmock.cpp
        test_mocking_free_function.cpp
    )
    
    foreach(file ${TEST_SOURCES})
        add_testcase(${file})
    endforeach()
endif()

