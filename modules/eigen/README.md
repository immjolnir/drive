# Eigen, a lightweight C++ template library for linear algebra.

* https://eigen.tuxfamily.org/dox/group__QuickRefPage.html


- Core `#include <Eigen/Core>`
Matrix and Array classes, basic linear algebra (including triangular and selfadjoint products), array manipulation

- Geometry #include <Eigen/Geometry> Transform, Translation, Scaling, Rotation2D and 3D rotations (Quaternion, AngleAxis)

- LU #include <Eigen/LU> Inverse, determinant, LU decompositions with solver (FullPivLU, PartialPivLU)


- Cholesky #include <Eigen/Cholesky> LLT and LDLT Cholesky factorization with solver


- Householder #include <Eigen/Householder> Householder transformations; this module is used by several linear algebra modules

- SVD #include <Eigen/SVD> SVD decompositions with least-squares solver (JacobiSVD, BDCSVD)


- QR #include <Eigen/QR> QR decomposition with solver (HouseholderQR, ColPivHouseholderQR, FullPivHouseholderQR)


- Eigenvalues #include <Eigen/Eigenvalues> Eigenvalue, eigenvector decompositions (EigenSolver, SelfAdjointEigenSolver, ComplexEigenSolver)


- Sparse #include <Eigen/Sparse> Sparse matrix storage and related basic linear algebra (SparseMatrix, SparseVector)


- `#include <Eigen/Dense>`
Includes Core, Geometry, LU, Cholesky, SVD, QR, and Eigenvalues header files

- `#include <Eigen/Eigen>`
Includes Dense and Sparse header files (the whole Eigen library)