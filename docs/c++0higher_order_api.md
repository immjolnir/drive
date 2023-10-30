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

