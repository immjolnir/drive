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

## vectorizable fixed-size Eigen object

In other words: 
you have probably in your code a class that has as a member a vectorizable fixed-size Eigen object, and you then dynamically allocated an object of that class.

By "vectorizable fixed-size Eigen object",  we mean an Eigen matrix or vector of fixed size, and whose size is a multiple of 128 bits. 

Examples include:
- Eigen::Vector2d
- Eigen::Vector4d
- Eigen::Vector4f
- Eigen::Matrix2d
- Eigen::Matrix2f
- Eigen::Matrix4d
- Eigen::Matrix4f
- Eigen::Transform3d
- Eigen::Transform3f

how How to fix this bug?


Very easy, you just need to put a `EIGEN_MAKE_ALIGNED_OPERATOR_NEW` macro in a public part of your class, like this:
```c++
class Foo{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
    Eigen::Vector2d data
};
```
- https://github.com/AIBluefisher/Eigen_Handbook/blob/master/EN/chap5_more.md

它是在 Eigen/src/Core/util/Memory.h 实现的
```c++
#if EIGEN_HAS_CXX17_OVERALIGN

// C++17 -> no need to bother about alignment anymore :)， 为什么这么说呢？
// c++17是怎么实现这个功能的呢？怎么做benchmark呢？

#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW_NOTHROW(NeedsToAlign)
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign)
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(Scalar, Size)

#else

#if EIGEN_MAX_ALIGN_BYTES != 0
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW_NOTHROW(NeedsToAlign)                                 \
    void* operator new(std::size_t size, const std::nothrow_t&) EIGEN_NO_THROW {              \
        EIGEN_TRY { return Eigen::internal::conditional_aligned_malloc<NeedsToAlign>(size); } \
        EIGEN_CATCH(...) { return 0; }                                                        \
    }
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign)                                                               \
    void* operator new(std::size_t size) { return Eigen::internal::conditional_aligned_malloc<NeedsToAlign>(size); }   \
    void* operator new[](std::size_t size) { return Eigen::internal::conditional_aligned_malloc<NeedsToAlign>(size); } \
    void operator delete(void* ptr) EIGEN_NO_THROW { Eigen::internal::conditional_aligned_free<NeedsToAlign>(ptr); }   \
    void operator delete[](void* ptr) EIGEN_NO_THROW { Eigen::internal::conditional_aligned_free<NeedsToAlign>(ptr); } \
    void operator delete(void* ptr, std::size_t /* sz */) EIGEN_NO_THROW {                                             \
        Eigen::internal::conditional_aligned_free<NeedsToAlign>(ptr);                                                  \
    }                                                                                                                  \
    void operator delete[](void* ptr, std::size_t /* sz */) EIGEN_NO_THROW {                                           \
        Eigen::internal::conditional_aligned_free<NeedsToAlign>(ptr);                                                  \
    }                                                                                                                  \
    /* in-place new and delete. since (at least afaik) there is no actual   */                                         \
    /* memory allocated we can safely let the default implementation handle */                                         \
    /* this particular case. */                                                                                        \
    static void* operator new(std::size_t size, void* ptr) { return ::operator new(size, ptr); }                       \
    static void* operator new[](std::size_t size, void* ptr) { return ::operator new[](size, ptr); }                   \
    void operator delete(void* memory, void* ptr) EIGEN_NO_THROW { return ::operator delete(memory, ptr); }            \
    void operator delete[](void* memory, void* ptr) EIGEN_NO_THROW { return ::operator delete[](memory, ptr); }        \
    /* nothrow-new (returns zero instead of std::bad_alloc) */                                                         \
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_NOTHROW(NeedsToAlign)                                                              \
    void operator delete(void* ptr, const std::nothrow_t&) EIGEN_NO_THROW {                                            \
        Eigen::internal::conditional_aligned_free<NeedsToAlign>(ptr);                                                  \
    }                                                                                                                  \
    typedef void eigen_aligned_operator_new_marker_type;
#else
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign)
#endif
// EIGEN_MAKE_ALIGNED_OPERATOR_NEW 我们代码中使用的就是这个
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(true)
#define EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(Scalar, Size)                                 \
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(                                                                          \
      bool(((Size) != Eigen::Dynamic) &&                                                                         \
           (((EIGEN_MAX_ALIGN_BYTES >= 16) && ((sizeof(Scalar) * (Size)) % (EIGEN_MAX_ALIGN_BYTES) == 0)) ||     \
            ((EIGEN_MAX_ALIGN_BYTES >= 32) && ((sizeof(Scalar) * (Size)) % (EIGEN_MAX_ALIGN_BYTES / 2) == 0)) || \
            ((EIGEN_MAX_ALIGN_BYTES >= 64) && ((sizeof(Scalar) * (Size)) % (EIGEN_MAX_ALIGN_BYTES / 4) == 0)))))

#endif

/*****************************************************************************
*** Implementation of conditionally aligned functions                      ***
*****************************************************************************/

/** \internal Allocates \a size bytes. If Align is true, then the returned ptr is 16-byte-aligned.
 * On allocation error, the returned pointer is null, and a std::bad_alloc is thrown.
 */
template <bool Align>
EIGEN_DEVICE_FUNC inline void* conditional_aligned_malloc(std::size_t size) {
    return aligned_malloc(size);
}

template <>
EIGEN_DEVICE_FUNC inline void* conditional_aligned_malloc<false>(std::size_t size) {
    check_that_malloc_is_allowed();

    EIGEN_USING_STD(malloc)
    void* result = malloc(size);

    if (!result && size) throw_std_bad_alloc();
    return result;
}

/** \internal Frees memory allocated with conditional_aligned_malloc */
template <bool Align>
EIGEN_DEVICE_FUNC inline void conditional_aligned_free(void* ptr) {
    aligned_free(ptr);
}

template <>
EIGEN_DEVICE_FUNC inline void conditional_aligned_free<false>(void* ptr) {
    EIGEN_USING_STD(free)
    free(ptr);
}

template <bool Align>
inline void* conditional_aligned_realloc(void* ptr, std::size_t new_size, std::size_t old_size) {
    return aligned_realloc(ptr, new_size, old_size);
}

template <>
inline void* conditional_aligned_realloc<false>(void* ptr, std::size_t new_size, std::size_t) {
    return std::realloc(ptr, new_size);
}

template<typename T>
EIGEN_DEVICE_FUNC EIGEN_ALWAYS_INLINE void check_size_for_overflow(std::size_t size)
{
  if(size > std::size_t(-1) / sizeof(T))
    throw_std_bad_alloc();
}

/** \internal Allocates \a size objects of type T. The returned pointer is guaranteed to have 16 bytes alignment.
 * On allocation error, the returned pointer is undefined, but a std::bad_alloc is thrown.
 * The default constructor of T is called.
 */
template <typename T>
EIGEN_DEVICE_FUNC inline T* aligned_new(std::size_t size) {
    check_size_for_overflow<T>(size);
    T* result = reinterpret_cast<T*>(aligned_malloc(sizeof(T) * size));
    EIGEN_TRY { return construct_elements_of_array(result, size); }
    EIGEN_CATCH(...) {
        aligned_free(result);
        EIGEN_THROW;
    }
    return result;
}

template <typename T, bool Align>
EIGEN_DEVICE_FUNC inline T* conditional_aligned_new(std::size_t size) {
    check_size_for_overflow<T>(size);
    T* result = reinterpret_cast<T*>(conditional_aligned_malloc<Align>(sizeof(T) * size));
    EIGEN_TRY { return construct_elements_of_array(result, size); }
    EIGEN_CATCH(...) {
        conditional_aligned_free<Align>(result);
        EIGEN_THROW;
    }
    return result;
}

/** \internal Deletes objects constructed with aligned_new
 * The \a size parameters tells on how many objects to call the destructor of T.
 */
template <typename T>
EIGEN_DEVICE_FUNC inline void aligned_delete(T* ptr, std::size_t size) {
    destruct_elements_of_array<T>(ptr, size);
    Eigen::internal::aligned_free(ptr);
}

/** \internal Deletes objects constructed with conditional_aligned_new
 * The \a size parameters tells on how many objects to call the destructor of T.
 */
template <typename T, bool Align>
EIGEN_DEVICE_FUNC inline void conditional_aligned_delete(T* ptr, std::size_t size) {
    destruct_elements_of_array<T>(ptr, size);
    conditional_aligned_free<Align>(ptr);
}
```

