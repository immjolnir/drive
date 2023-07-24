list(APPEND MODULES project learnopencv vot mlap)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()
