list(APPEND MODULES project)

foreach(m ${MODULES})
    add_subdirectory(${m})
endforeach()
