list(APPEND MODULES
    basis # C++ Programming Basis
    common project learnopencv vot mlap)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()
