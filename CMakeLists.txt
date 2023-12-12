cmake_minimum_required(VERSION 3.20)
project(drive CXX)

option(ARCHETYPE_DEVELOPMENT_BUILD "Build tests, enable warnings, enable Werror, etc" ON)
option(ENABLE_DOXYGEN "Generating documentation from annotated C++ sources" OFF)
option(ENABLE_SANITIZER "Sanitizer: address, memory" OFF)


# The building is awful and scared. the projects are disabled by default.
option(ENABLE_MODULE_BASIS "C++ Programming Basic Skips" OFF)
option(ENABLE_MODULE_EXPLORING "Exploring C++ New Boundary" OFF)
option(ENABLE_MODULE_LEARN_OPENCV "Learning OpenCV" OFF)
option(ENABLE_MODULE_VOT "vot" OFF)
option(ENABLE_MODULE_MLAP "mlap" OFF)
option(ENABLE_PROTOBUFF "Google Protobuffer" OFF)
option(ENABLE_3D "Three-Dimensions" OFF)
option(ENABLE_MYTEST "mytest copied from gtest" OFF)
option(ENABLE_EIGEN "Learning Eigen" OFF)
option(ENABLE_BOOST "Learing Boost's libraries" OFF)
option(ENABLE_LOGGING "Learing logging module" OFF)
option(ENABLE_ARROW "A Mini Arrow module" OFF)
option(ENABLE_APOLLO "A Mini Apollo module" OFF)
option(ENABLE_JSON "Jsoncpp module" OFF)
option(ENABLE_ALGOS "Struct&Algorithm module" OFF)
option(ENABLE_LEETCODE "LeetCode module" OFF)
option(ENABLE_ASSEMBLER "Assembler module" OFF)


include(CMakeDependentOption)
# BUILD_TESTING is an builtin environment variable.
# https://cmake.org/cmake/help/latest/command/add_test.html
cmake_dependent_option(BUILD_TESTING "Build the SelfTest project" ON "ARCHETYPE_DEVELOPMENT_BUILD" OFF)
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

string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" test_name "fake-dir/test_employee.cpp")
message(STATUS ">>> ${test_name}") # test_employee

string(REGEX REPLACE "([a-zA-Z0-9_ ]+)(\.cpp)" "\\1" test_name "test_employee.cpp")
message(STATUS ">>> ${test_name}") # test_employee

string(REGEX REPLACE "([a-zA-Z0-9_ ]+)(\.cpp)" "\\1" test_name "fake_dir/test_employee.cpp")
message(STATUS ">>> ${test_name}") # fake_dir/test_employee

# GFlags
find_package(Gflags REQUIRED)
message(STATUS "GFLAGS_INCLUDE_DIRS: ${GFLAGS_INCLUDE_DIRS}")
message(STATUS "GFLAGS_LIBRARIES: ${GFLAGS_LIBRARIES}")

# GLog
find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})
message(STATUS "GLOG_INCLUDE_DIRS: ${GLOG_INCLUDE_DIRS}")
message(STATUS "GLOG_LIBRARIES: ${GLOG_LIBRARIES}")

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

# jsoncpp
include(${CMAKE_FILES}/FindJsonCpp.cmake)
find_package(JsonCpp REQUIRED)
include_directories(${JsonCpp_INCLUDE_DIRS})
message(STATUS "JsonCpp_INCLUDE_DIRS: ${JsonCpp_INCLUDE_DIRS}")
message(STATUS "JsonCpp_LIBRARIES: ${JsonCpp_LIBRARIES}")

# Boost
# -- Boost_LIBRARIES = Boost::atomic;Boost::chrono;Boost::container;Boost::context;Boost::coroutine;Boost::date_time;Boost::exception;Boost::fiber;Boost::filesystem;Boost::graph;Boost::graph_parallel;Boost::iostreams;Boost::json;Boost::locale;Boost::log;Boost::log_setup;Boost::math_c99;Boost::math_c99f;Boost::math_c99l;Boost::math_tr1;Boost::math_tr1f;Boost::math_tr1l;Boost::mpi;Boost::mpi_python;Boost::nowide;Boost::numpy;Boost::prg_exec_monitor;Boost::program_options;Boost::python;Boost::random;Boost::regex;Boost::serialization;Boost::stacktrace_addr2line;Boost::stacktrace_backtrace;Boost::stacktrace_basic;Boost::stacktrace_noop;Boost::system;Boost::test_exec_monitor;Boost::thread;Boost::timer;Boost::type_erasure;Boost::unit_test_framework;Boost::wave;Boost::wserialization
# find_package(Boost REQUIRED COMPONENTS ALL)
find_package(Boost REQUIRED COMPONENTS filesystem serialization regex)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

# yaml-cpp is a YAML parser and emitter in C++ matching the YAML 1.2 spec.
find_package(yaml-cpp REQUIRED)
message(STATUS "YAML_CPP_LIBRARIES = ${YAML_CPP_LIBRARIES}")

# fmt
# https://github.com/fmtlib/fmt/blob/master/test/find-package-test/CMakeLists.txt
find_package(FMT REQUIRED)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
message(STATUS "EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR}")
message(STATUS "EIGEN3_VERSION: ${EIGEN3_VERSION}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if(ARCHETYPE_DEVELOPMENT_BUILD) # This statement shoundn't work!
    message(STATUS  "Stop here")
endif()

# https://stackoverflow.com/questions/35744647/disabling-cmake-option-has-no-effect
# if (NEED_GUI MATCHES ON)
# is the appropriate usage rather than
# if (NEED_GUI)
#
# https://stackoverflow.com/questions/44719963/cmake-default-value-for-options-doesnt-work
if(ARCHETYPE_DEVELOPMENT_BUILD MATCHES ON) # This statement work!
    message(STATUS "Stop here 2")
endif()

# by default build debug version
if(NOT DEFINED CMAKE_BUILD_TYPE)
    message(STATUS "Fallback: set the CMAKE_BUILD_TYPE=Debug, why it does not work?")
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS ">>> (before) CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}") # why it is empty?
set(CMAKE_BUILD_TYPE Debug) # use debug as default
message(STATUS ">>> (after) CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}") # why it is empty?

if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
    message(STATUS ">>> Success to set CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

include(${CMAKE_FILES}/CompilerWarnings.cmake)
# These options should ideally be consistent across C++ repos.
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set_cxx_compiler_warnings()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20") # use c++20
# error: conversion from 'double' to 'float' may change value [-Werror=float-conversion]
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wsign-conversion -Werror")
# 开启额外的警告 这些选项分别启用所有基本警告和额外警告，帮助发现潜在的问题。
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 遵守严格标准,此选项要求GCC遵循ISO C/C++标准，并发出违反这些标准的警告
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

#-march=target 和 -mtune=target：CPU架构优化 为特定的处理器架构进行优化编译和调整优化目标。
#??

# Changing the linker
# https://stackoverflow.com/questions/1867745/cmake-use-a-custom-linker
# https://github.com/rui314/mold#how-to-use
# For GCC 12.1.0 or later: pass -fuse-ld=mold
# lld, gold, mold
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

include(${CMAKE_FILES}/utility.cmake)

# https://clang.llvm.org/docs/UsersManual.html#controlling-code-generation
# -fsanitize=address: AddressSanitizer, a memory error detector.
#
# -fsanitize=thread: ThreadSanitizer, a data race detector.
#
# -fsanitize=memory: MemorySanitizer, a detector of uninitialized reads. Requires instrumentation of all program code.
#
# -fsanitize=undefined: UndefinedBehaviorSanitizer, a fast and compatible undefined behavior checker.
#
# -fsanitize=dataflow: DataFlowSanitizer, a general data flow analysis.
#
# -fsanitize=cfi: control flow integrity checks. Requires -flto.
#
# -fsanitize=kcfi: kernel indirect call forward-edge control flow integrity.
#
# -fsanitize=safe-stack: safe stack protection against stack-based memory corruption errors.
if(ENABLE_SANITIZER)
    include(${CMAKE_FILES}/EnableSanitizer.cmake)
    enable_sanitizer(address)
endif(ENABLE_SANITIZER)

add_header()

# Basic paths
set(CATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR})


if(BUILD_TESTING)
    # Build the unit test binary, and add all of the unit tests.
    # https://cmake.org/cmake/help/latest/manual/ctest.1.html
    # Why not work?
    # set(CTEST_OUTPUT_ON_FAILURE ON)
    # set(CTEST_PROGRESS_OUTPUT ON)
    # set(CTEST_PARALLEL_LEVEL 5)
    # set(CTEST_BUILD_COMMAND "ctest --rerun-failed --output-on-failure")
    include(CTest)
    # https://www.yii666.com/blog/448821.html
    # include(GoogleTest)
    # gtest_add_tests(TARGET t)
    # gtest_discover_tests(t)

    # Fix for such log:
    # Output from these tests are in: /iwork/drive/build/modules/vot/Testing/Temporary/LastTest.log
    # Use "--rerun-failed --output-on-failure" to re-run the failed cases verbosely.
    list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
    # For quickly debugging, run `ctest --rerun-failed` or `CTEST_OUTPUT_ON_FAILURE=1 ctest` under the test folder.


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
    # https://cmake.org/cmake/help/latest/command/enable_testing.html
    # This command is automatically invoked when the CTest module is included, except if the BUILD_TESTING option is turned off.
    # The CTest module invokes enable_testing automatically unless BUILD_TESTING is set to OFF.
    # enable_testing()
endif(BUILD_TESTING)

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
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# We need to bring-in the variables defined there to this scope
add_subdirectory(modules)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

add_footer()
