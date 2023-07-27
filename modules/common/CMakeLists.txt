project(common CXX)

# Policy CMP0076 is not set: target_sources() command converts relative paths to absolute. 
# Run "cmake --help-policy CMP0076" for policy details. 
# Use the cmake_policy command to set the policy and suppress this warning.
set(SOURCES_DIR ${CATCH_DIR}/modules/${PROJECT_NAME})

set(CLI_SOURCES
    ${SOURCES_DIR}/component_entry.cpp
)

set(ALL_FILES
    ${CLI_SOURCES}
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${ALL_FILES}
)

target_include_directories(${PROJECT_NAME} BEFORE
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)