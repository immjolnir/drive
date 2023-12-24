
#list(APPEND MODULES common demo_project)

if(ENABLE_MODULE_BASIS)
    list(APPEND MODULES
        basis
        forward_declaration/fix0v1
        forward_declaration/issuecase
    )
endif()

if(ENABLE_MODULE_EXPLORING)
    list(APPEND MODULES exploring)
endif()

if(ENABLE_MODULE_LEARN_OPENCV)
    list(APPEND MODULES learnopencv)
endif()

if(ENABLE_MODULE_VOT)
    list(APPEND MODULES common)
    list(APPEND MODULES vot)
endif()

if(ENABLE_MODULE_MLAP)
    list(APPEND MODULES mlap)
endif()

if (ENABLE_PROTOBUFF)
    list(APPEND MODULES google_protobuf)
endif()

if (ENABLE_GTEST)
    list(APPEND MODULES google_gtest)
endif()

if (ENABLE_3D)
    list(APPEND MODULES three_dimensions)
endif()

if (ENABLE_MM)
    list(APPEND MODULES memory_management)
endif()

if (ENABLE_ROS)
    list(APPEND MODULES ros)
endif()

if (ENABLE_MYTEST)
    list(APPEND MODULES mytest)
endif()

if (ENABLE_EIGEN)
    list(APPEND MODULES eigen)
endif()

if (ENABLE_JSON)
    list(APPEND MODULES jsoncpp)
endif()

if (ENABLE_BOOST)
    list(APPEND MODULES boostorg)
endif()

if (ENABLE_LOGGING)
    list(APPEND MODULES logging)
endif()

if (ENABLE_ARROW)
    list(APPEND MODULES miniarrow)
endif()

if (ENABLE_ALGOS)
    list(APPEND MODULES algos)
endif()

if (ENABLE_LEETCODE)
    list(APPEND MODULES leetcode)
endif()

if (ENABLE_ASSEMBLER)
    list(APPEND MODULES assembler)
endif()

if (ENABLE_DESIGN_PATTERN)
    list(APPEND MODULES design_pattern)
endif()

if (ENABLE_SPARROW)
    list(APPEND MODULES house_sparrow)
endif()

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()
