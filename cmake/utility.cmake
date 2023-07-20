# utility.cmake

function(add_example src)
    # https://cmake.org/cmake/help/latest/command/get_filename_component.html
    get_filename_component(example ${src} NAME_WLE)
    message(STATUS "Creating example ${example}")
    add_executable(${example} ${src})
    target_include_directories(${example} BEFORE
        PUBLIC
            ${CMAKE_SOURCE_DIR}/modules
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
    set(additional_libs ${ARGN})
    target_link_libraries(${example}
        ${additional_libs}
    )
endfunction()

# Add unit tests
function(add_testcase unittest_file)
    get_filename_component(testcase ${unittest_file} NAME_WLE)
    message(STATUS "Creating unittest ${testcase}")
    add_executable(${testcase} ${unittest_file})
    # target_compile_features(${testcase} PUBLIC cxx_std_17)
    target_include_directories(${testcase} BEFORE
        PUBLIC
            ${CMAKE_SOURCE_DIR}/modules
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
    set(additional_libs ${ARGN})
    target_link_libraries(${testcase}
        ${PROJECT_NAME}
        ${GTEST_BOTH_LIBRARIES}
        ${GMOCK_BOTH_LIBRARIES}
        ${additional_libs}
    )
    add_test(NAME ${PROJECT_NAME}/${testcase}
        COMMAND ${testcase}
        # WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
    )
endfunction()
