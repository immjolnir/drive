cmake_minimum_required(VERSION 3.10)
project(modern_cpp_archetype CXX)

option(ARCHETYPE_DEVELOPMENT_BUILD "Build tests, enable warnings, enable Werror, etc" OFF)
option(ENABLE_DOXYGEN "Generating documentation from annotated C++ sources" OFF)

include(CMakeDependentOption)
cmake_dependent_option(ARCHETYPE_BUILD_TESTING "Build the SelfTest project" ON "ARCHETYPE_DEVELOPMENT_BUILD" OFF)
cmake_dependent_option(BUILD_EXAMPLES "Build code examples" OFF "ARCHETYPE_DEVELOPMENT_BUILD" OFF)
cmake_dependent_option(ARCHETYPE_ENABLE_COVERAGE "Generate coverage for codecov.io" OFF "ARCHETYPE_DEVELOPMENT_BUILD" OFF)
cmake_dependent_option(ARCHETYPE_ENABLE_WERROR "Enables Werror during build" ON "ARCHETYPE_DEVELOPMENT_BUILD" OFF)

# Contains various Find files, code coverage, 3rd party library FetchContent scripts,
# and the project's Package Configuration script.
set(CMAKE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# cmake module path
# https://github.com/facebook/fbthrift/blob/main/CMakeLists.txt
# https://github.com/launchdarkly/c-server-sdk/blob/main/CMakeLists.txt
# Use the CMAKE_FILES directory to find these packages first
list(APPEND CMAKE_MODULE_PATH "${CMAKE_FILES}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")



string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" test_name test_employee.cpp)
message(STATUS ">>> ${test_name}") # test_employee.cpp

string(REGEX REPLACE "([a-zA-Z0-9_ ]+)(\.cpp)" "\\1" test_name test_employee.cpp)
message(STATUS ">>> ${test_name}") # test_employee

# GFlags
find_package(Gflags REQUIRED)
message(STATUS "GFLAGS_INCLUDE_DIRS: ${GFLAGS_INCLUDE_DIRS}")
message(STATUS "GFLAGS_LIBRARIES: ${GFLAGS_LIBRARIES}")

# GLog
find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})
message(STATUS "GLOG_INCLUDE_DIRS: ${GLOG_INCLUDE_DIRS}")
message(STATUS "GLOG_LIBRARIES: ${GLOG_LIBRARIES}")

if(ARCHETYPE_DEVELOPMENT_BUILD)
    set(CMAKE_BUILD_TYPE Debug)
endif(ARCHETYPE_DEVELOPMENT_BUILD)

include(${CMAKE_FILES}/utility.cmake)

# Basic paths
set(CATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(ARCHETYPE_BUILD_TESTING)
    # GTest
    # https://cmake.org/cmake/help/latest/module/FindGTest.html
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    message(STATUS "GTEST_LIBRARIES: ${GTEST_LIBRARIES}")
    message(STATUS "GTEST_MAIN_LIBRARIES: ${GTEST_MAIN_LIBRARIES}")
    # GTest::gtest;GTest::gtest_main
    message(STATUS "GTEST_BOTH_LIBRARIES: ${GTEST_BOTH_LIBRARIES}")

    # GMock
    find_package(GMock REQUIRED)
    include_directories(${GMOCK_INCLUDE_DIRS})
    message(STATUS "GMOCK_LIBRARIES: ${GMOCK_LIBRARIES}")
    message(STATUS "GMOCK_MAIN_LIBRARIES: ${GMOCK_MAIN_LIBRARIES}")
    # /usr/lib/x86_64-linux-gnu/libgmock.a;/usr/lib/x86_64-linux-gnu/libgmock_main.a
    message(STATUS "GMOCK_BOTH_LIBRARIES: ${GMOCK_BOTH_LIBRARIES}")

    # Build the unit test binary, and add all of the unit tests.
    include(CTest)

    if(ARCHETYPE_ENABLE_COVERAGE)
        message(STATUS "Code coverage is enabled and provided with GCC.")
        # target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
        # target_link_options(${PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
        include(${CMAKE_FILES}/CodeCoverage.cmake)
        append_coverage_compiler_flags()
        # set(COVERAGE_EXCLUDES
        #     '/usr/include/*'
        #     '/usr/lib/*
        # )
        # Disable it for mismatch packages.
        # https://github.com/gcovr/gcovr
        # https://github.com/pallets/jinja
        # setup_target_for_coverage_gcovr_html(NAME coverage)

        # https://stackoverflow.com/questions/29867756/cmake-create-coverage-target
        add_custom_target(coverage
            COMMAND bash ${CMAKE_SOURCE_DIR}/do_coverage.sh
        )
    endif()

    enable_testing()
endif(ARCHETYPE_BUILD_TESTING)

if(ENABLE_DOXYGEN)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)

    find_package(Doxygen REQUIRED dot)
    doxygen_add_docs(doxygen-docs ${PROJECT_SOURCE_DIR})
    message(STAUS "Doxygen has been setup and documentation is now available.")
endif()
############################### END of Basic Setting ###############################

# We need to bring-in the variables defined there to this scope
add_subdirectory(modules)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
