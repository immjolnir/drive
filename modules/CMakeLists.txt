list(APPEND MODULES
    basis # C++ Programming Basis
    exploring # Explore C++
    common demo_project learnopencv vot mlap)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()
