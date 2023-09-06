list(APPEND MODULES common demo_project)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()

if(ENABLE_MODULE_BASIS)
    add_subdirectory(basis)
    add_subdirectory(forward_declaration/fix0v1)
    add_subdirectory(forward_declaration/issuecase)
endif()

if(ENABLE_MODULE_EXPLORING)
    add_subdirectory(exploring)
endif()

if(ENABLE_MODULE_LEARN_OPENCV)
    add_subdirectory(learnopencv)
endif()

if(ENABLE_MODULE_VOT)
    add_subdirectory(vot)
endif()

if(ENABLE_MODULE_MLAP)
    add_subdirectory(mlap)
endif()

if (ENABLE_PROTOBUFF)
    add_subdirectory(google_protobuf)
endif()

if (ENABLE_3D)
    add_subdirectory(three_dimensions)
endif()

if (ENABLE_MYTEST)
    add_subdirectory(mytest)
endif()