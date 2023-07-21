list(APPEND MODULES project vot)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()
