# Higher Order APIs

## src/google/protobuf/port_def.inc

- PROTOBUF_PREDICT_TRUE
- GOOGLE_PREDICT_FALSE

```c++
#ifdef GOOGLE_PREDICT_TRUE
#define PROTOBUF_PREDICT_TRUE GOOGLE_PREDICT_TRUE
#else
#ifdef __GNUC__
// Provided at least since GCC 3.0.
#define PROTOBUF_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define PROTOBUF_PREDICT_TRUE(x) (x)
#endif
#endif

#ifdef GOOGLE_PREDICT_FALSE
#define PROTOBUF_PREDICT_FALSE GOOGLE_PREDICT_FALSE
#else
#ifdef __GNUC__
// Provided at least since GCC 3.0.
#define PROTOBUF_PREDICT_FALSE(x) (__builtin_expect(x, 0))
#else
#define PROTOBUF_PREDICT_FALSE(x) (x)
#endif
#endif
```


- http://blog.man7.org/2012/10/how-much-do-builtinexpect-likely-and.html
- https://stackoverflow.com/questions/7346929/what-is-the-advantage-of-gccs-builtin-expect-in-if-else-statements

- PROTOBUF_DEPRECATED_MSG
```c++
#if defined(__GNUC__) || defined(__clang__)
#define PROTOBUF_DEPRECATED __attribute__((deprecated))
#define PROTOBUF_DEPRECATED_ENUM __attribute__((deprecated))
#define PROTOBUF_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define PROTOBUF_DEPRECATED __declspec(deprecated)
#define PROTOBUF_DEPRECATED_ENUM
#define PROTOBUF_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#endif
```
- GOOGLE_ATTRIBUTE_ALWAYS_INLINE
```c++
// ----------------------------------------------------------------------------
// Annotations:  Some parts of the code have been annotated in ways that might
//   be useful to some compilers or tools, but are not supported universally.
//   You can #define these annotations yourself if the default implementation
//   is not right for you.

#ifdef GOOGLE_ATTRIBUTE_ALWAYS_INLINE
#define PROTOBUF_ALWAYS_INLINE GOOGLE_ATTRIBUTE_ALWAYS_INLINE
#else
#if defined(__GNUC__) && \
    (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
// For functions we want to force inline.
// Introduced in gcc 3.1.
#define PROTOBUF_ALWAYS_INLINE __attribute__((always_inline))
#else
// Other compilers will have to figure it out for themselves.
#define PROTOBUF_ALWAYS_INLINE
#endif
#endif
```

- PROTOBUF_NOINLINE
```c++
#ifdef GOOGLE_ATTRIBUTE_NOINLINE
#define PROTOBUF_NOINLINE GOOGLE_ATTRIBUTE_NOINLINE
#else
#if defined(__GNUC__) && \
    (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
// For functions we want to force not inline.
// Introduced in gcc 3.1.
#define PROTOBUF_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
// Seems to have been around since at least Visual Studio 2005
#define PROTOBUF_NOINLINE __declspec(noinline)
#else
// Other compilers will have to figure it out for themselves.
#define PROTOBUF_NOINLINE
#endif
#endif

```