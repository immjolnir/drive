project(mlap CXX)

add_header()

set(chapter1
    chapter1/cv_Mat_1.cpp
    chapter1/cv_Mat_2.cpp
    chapter1/cv_Point.cpp
    chapter1/cv_Rect.cpp
    chapter1/cv_RotateRect.cpp
    chapter1/cv_Vector.cpp
)

set(chapter2
    chapter2/2-1.cpp
    chapter2/2-2.cpp
)

set(chapter4
    chapter4_k-means_KNN/k-means.cpp
    chapter4_k-means_KNN/KNN.cpp
)

set(ALL_FILES
    ${chapter1}
    ${chapter2}
    ${chapter4}
)

set(RESOURCES
    images
    Datasets
)
add_resource_tunnel(${RESOURCES})

foreach(file ${ALL_FILES})
    add_example(${file})
endforeach()

add_footer()

