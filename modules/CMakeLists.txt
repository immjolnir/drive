# The building is awful and scared. the projects are disabled by default.

option(ENABLE_MODULE_BASIS "C++ Programming Basic Skips" OFF)
option(ENABLE_MODULE_EXPLORING "Exploring C++ New Boundary" OFF)
option(ENABLE_MODULE_LEARN_OPENCV "Learning OpenCV" OFF)
option(ENABLE_MODULE_VOT "vot" OFF)
option(ENABLE_MODULE_MLAP "mlap" OFF)

list(APPEND MODULES common demo_project)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()

if(ENABLE_MODULE_BASIS)
    add_subdirectory(basis)
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

