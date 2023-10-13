# Atomic


```c++
// Based on "C++ Atomic Types and Operations" by Hans Boehm and Lawrence Crowl.
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2427.html

#if __cplusplus >= 201703L
#define __cpp_lib_atomic_is_always_lock_free 201603L
#endif

#if __cplusplus <= 201703L
# define _GLIBCXX20_INIT(I)
#else
# define _GLIBCXX20_INIT(I) = I
#endif

template <typename _Tp>
struct atomic;

/**
 *  @brief Generic atomic type, primary class template.
 *
 *  @tparam _Tp  Type to be made atomic, must be trivially copyable.
 */
template <typename _Tp>
struct atomic {
    using value_type = _Tp;

  private:
    // Align 1/2/4/8/16-byte types to at least their size.
    static constexpr int _S_min_alignment = (sizeof(_Tp) & (sizeof(_Tp) - 1)) || sizeof(_Tp) > 16 ? 0 : sizeof(_Tp);

    static constexpr int _S_alignment = _S_min_alignment > alignof(_Tp) ? _S_min_alignment : alignof(_Tp);

    alignas(_S_alignment) _Tp _M_i _GLIBCXX20_INIT(_Tp()); // 进行初始化 since c++17

    static_assert(__is_trivially_copyable(_Tp), "std::atomic requires a trivially copyable type");

    static_assert(sizeof(_Tp) > 0, "Incomplete or zero-sized types are not supported");

#if __cplusplus > 201703L
    static_assert(is_copy_constructible_v<_Tp>);
    static_assert(is_move_constructible_v<_Tp>);
    static_assert(is_copy_assignable_v<_Tp>);
    static_assert(is_move_assignable_v<_Tp>);
#endif
  public:
    atomic() = default;
    ~atomic() noexcept = default;
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;

    constexpr atomic(_Tp __i) noexcept : _M_i(__i) {}

    operator _Tp() const noexcept { return load(); }
    operator _Tp() const volatile noexcept { return load(); }

    _Tp operator=(_Tp __i) noexcept { store(__i); return __i; }
    _Tp operator=(_Tp __i) volatile noexcept { store(__i); return __i; }
};

/// Partial specialization for pointer types.
template <typename _Tp>
struct atomic<_Tp*> {
    ...
};

/// Explicit specialization for int.
template <>
struct atomic<int> : __atomic_base<int> {
    ...
};
```