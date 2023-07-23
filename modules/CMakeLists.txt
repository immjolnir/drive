list(APPEND MODULES project vot mlap)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()
