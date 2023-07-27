# utility.cmake

macro(add_header)
  message(STATUS "========== START ${PROJECT_NAME} ==========")
  message(STATUS "  PROJECT_NAME: ${PROJECT_NAME}")
  message(STATUS "  PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
  message(STATUS "  PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

  message(STATUS "  CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")
  message(STATUS "  CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
  message(STATUS "  CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

  message(STATUS "  CMAKE_CURRENT_LIST_FILE: ${CMAKE_CURRENT_LIST_FILE}")
  message(STATUS "  CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
  message(STATUS "  CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")


  message(STATUS "  CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
  message(STATUS "  CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
  message(STATUS "  CMAKE_INSTALL_RPATH: ${CMAKE_INSTALL_RPATH}")
  message(STATUS "  CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

  message(STATUS "  CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  message(STATUS "  RUNTIME_OUTPUT_DIRECTORY: ${RUNTIME_OUTPUT_DIRECTORY}")

  message(STATUS "  CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
  message(STATUS "========================================")
endmacro()

macro(add_footer)
  message(STATUS "========== END ${PROJECT_NAME} ==========")
endmacro()

# https://github.com/Kitware/CMake/blob/master/Modules/ExternalData.cmake
function(_ExternalData_random var)
  string(RANDOM LENGTH 6 random)
  set("${var}" "${random}" PARENT_SCOPE)
endfunction()

# https://cmake.org/cmake/help/latest/command/file.html
function(add_resource_tunnel)

    # file(RELATIVE_PATH images_path ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/images)

    set(resources ${ARGN})
    foreach(obj ${resources})
        # https://cmake.org/cmake/help/latest/command/if.html#is-absolute
        if(NOT IS_ABSOLUTE ${obj})
            set(src ${PROJECT_SOURCE_DIR}/${obj})
            set(dst ${PROJECT_BINARY_DIR}/${obj})
        else()
            get_filename_component(ObjName ${obj} NAME)
            set(src ${obj})
            set(dst ${PROJECT_BINARY_DIR}/${ObjName})
        endif()

        # https://cmake.org/cmake/help/latest/command/if.html#exists
        if(NOT EXISTS ${src})
            message(FATAL_ERROR "${src} dosen't exist!")
        endif()

        # https://cmake.org/cmake/help/latest/command/file.html#relative-path
        file(RELATIVE_PATH relative_path_to_src ${PROJECT_BINARY_DIR} ${src})
        message(STATUS "<<< relative path=${relative_path_to_src}")

        _ExternalData_random(random)
        set(tmp "${dst}.tmp${random}")
        file(CREATE_LINK ${relative_path_to_src} ${tmp} RESULT returncode SYMBOLIC)
        if(returncode)
            file(REMOVE "${tmp}")
            message(FATAL_ERROR "Failed to create:\n  \"${tmp}\"\nfrom:\n  \"${obj}\"\nwith error:\n  ${returncode}")
        endif()
        # Atomically create/replace the real destination.
        file(RENAME "${tmp}" "${dst}")
    endforeach()
endfunction()

function(add_example src)
    # https://cmake.org/cmake/help/latest/command/get_filename_component.html
    get_filename_component(example ${src} NAME_WLE)
    get_filename_component(output_dir ${src} DIRECTORY)
    # string(REGEX REPLACE "([a-zA-Z0-9_ ]+)(\.cpp)" "\\1" example "${src}")
    message(STATUS "Creating example ${example}")
    add_executable(${example} ${src})
    # set_target_warnings(${example})
    target_include_directories(${example} BEFORE
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/modules>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
    set(additional_libs ${ARGN})
    target_link_libraries(${example}
        ${additional_libs}
        ${OpenCV_LIBS}
    )

    # https://blog.csdn.net/MacKendy/article/details/122549819
    set_target_properties(${example} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${output_dir}")
endfunction()

# Add unit tests
function(add_testcase unittest_file)
    get_filename_component(testcase ${unittest_file} NAME_WLE)
    message(STATUS "Creating unittest ${testcase}")
    add_executable(${testcase} EXCLUDE_FROM_ALL ${unittest_file})
    # set_target_warnings(${testcase})
    # target_compile_features(${testcase} PUBLIC cxx_std_17)
    target_include_directories(${testcase} BEFORE
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/modules>
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
