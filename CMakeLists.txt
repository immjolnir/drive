cmake_minimum_required(VERSION 3.20)
project(drive CXX)

option(ARCHETYPE_DEVELOPMENT_BUILD "Build tests, enable warnings, enable Werror, etc" OFF)
option(ENABLE_DOXYGEN "Generating documentation from annotated C++ sources" OFF)

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

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if(ARCHETYPE_DEVELOPMENT_BUILD)
    set(CMAKE_BUILD_TYPE Debug)
endif(ARCHETYPE_DEVELOPMENT_BUILD)

include(${CMAKE_FILES}/CompilerWarnings.cmake)
# These options should ideally be consistent across C++ repos.
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set_cxx_compiler_warnings()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

include(${CMAKE_FILES}/utility.cmake)

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
