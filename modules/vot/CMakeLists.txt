project(vot CXX)

message(STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")

add_header()

# Policy CMP0076 is not set: target_sources() command converts relative paths to absolute. 
# Run "cmake --help-policy CMP0076" for policy details. 
# Use the cmake_policy command to set the policy and suppress this warning.
set(SOURCES_DIR ${CATCH_DIR}/modules/${PROJECT_NAME})

SET(CONFIG_FILES
    ${SOURCES_DIR}/config.cpp
)

SET(DATA_FILES
    ${SOURCES_DIR}/data.cpp
)
SET(ALLF_FILES
    ${CONFIG_FILES}
    ${DATA_FILES}
)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME}
   PRIVATE
       ${ALLF_FILES}
)
target_include_directories(${PROJECT_NAME} BEFORE
   PUBLIC
       $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${GLOG_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
)

add_resource_tunnel(config.yaml 
	#TB-50
)
add_example(data_loader.cpp)
add_example(vot_main.cpp
    ${PROJECT_NAME}
    common)

# Adds unit tests, and if enabled, cmake project tests.
if (ARCHETYPE_BUILD_TESTING)
    message(STATUS "Adding tests under ${PROJECT_NAME}Tests...")

    set(TEST_SOURCES
        test_config.cpp
        test_data.cpp
    )
    foreach(file ${TEST_SOURCES})
        add_testcase(${file})
    endforeach()
endif()
add_footer()
