# Utility

- src/google/protobuf/message_lite.h
```c++
// This type wraps a variable whose constructor and destructor are explicitly
// called. It is particularly useful for a global variable, without its
// constructor and destructor run on start and end of the program lifetime.
// This circumvents the initial construction order fiasco, while keeping
// the address of the empty string a compile time constant.
//
// Pay special attention to the initialization state of the object.
// 1. The object is "uninitialized" to begin with.
// 2. Call Construct() or DefaultConstruct() only if the object is
//    uninitialized. After the call, the object becomes "initialized".
// 3. Call get() and get_mutable() only if the object is initialized.
// 4. Call Destruct() only if the object is initialized.
//    After the call, the object becomes uninitialized.
template <typename T>
class ExplicitlyConstructed {
 public:
  void DefaultConstruct() { new (&union_) T(); }

  template <typename... Args>
  void Construct(Args&&... args) {
    new (&union_) T(std::forward<Args>(args)...);
  }

  void Destruct() { get_mutable()->~T(); }

  constexpr const T& get() const { return reinterpret_cast<const T&>(union_); }
  T* get_mutable() { return reinterpret_cast<T*>(&union_); }

 private:
  // Prefer c++14 aligned_storage, but for compatibility this will do.
  union AlignedUnion {
    char space[sizeof(T)];
    int64 align_to_int64;
    void* align_to_ptr;
  } union_;
};

```

- src/google/protobuf/stubs/strutil.h

- src/google/protobuf/stubs/stringpiece.h
string_view alike.