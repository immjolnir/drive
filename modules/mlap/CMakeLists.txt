project(mlap CXX)

set(SOURCES_DIR ${CATCH_DIR}/modules/${PROJECT_NAME})

set(chapter1
    ${SOURCES_DIR}/chapter1/cv_Mat_1.cpp
    ${SOURCES_DIR}/chapter1/cv_Mat_2.cpp
    ${SOURCES_DIR}/chapter1/cv_Point.cpp
    ${SOURCES_DIR}/chapter1/cv_Rect.cpp
    ${SOURCES_DIR}/chapter1/cv_RotateRect.cpp
    ${SOURCES_DIR}/chapter1/cv_Vector.cpp
)

set(chapter2
    ${SOURCES_DIR}/chapter2/2-1.cpp
    ${SOURCES_DIR}/chapter2/2-2.cpp
)

set(chapter4
    ${SOURCES_DIR}/chapter4_k-means_KNN/k-means.cpp
)

set(ALL_FILES
    ${chapter1}
    ${chapter2}
    ${chapter4}
)

foreach(file ${ALL_FILES})
    add_example(${file})
endforeach()

