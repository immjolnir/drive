/*
https://github.com/akrzemi1/Optional.git
*/

#include <iostream>
#include <string>
#include <type_traits>

namespace std2
{
    constexpr struct trivial_init_t {
    } trivial_init{};

    static_assert(sizeof(trivial_init_t) == 1, "The size of an empty struct is 1 B");

    constexpr struct in_place_t {
    } in_place{};

    template <class T>
    union storage_t {
        unsigned char dummy_;
        T value_;

        constexpr storage_t(trivial_init_t) noexcept : dummy_(){};

        template <class... Args>
        constexpr storage_t(Args&&... args) : value_(std::forward<Args>(args)...) {}

        // note: 'std2::storage_t<T>::~storage_t() [with T = B]' is implicitly deleted because the default definition
        // would be ill-formed:
        //   29 | ~storage_t() = default;
        //      | ^
        // ~storage_t() = default;
        ~storage_t() {}
    };

    // use  std::aligned_union_t<1, G, H> storage;

    template <class T>
    struct optional {
      public:
        typedef T value_type;
        // 20.5.5.1, constructors
        constexpr optional() noexcept : init_(false), storage_(trivial_init){};

        constexpr optional(const T& v) : init_(true), storage_(v) {}

        constexpr optional(T&& v) : init_(true), storage_(std::forward<T>(v)) {}

        template <class... Args>
        explicit constexpr optional(Args&&... args) {
            initialize(std::forward<Args>(args)...);
        }

        constexpr bool initialized() const noexcept { return init_; }

        typename std::remove_const<T>::type* dataptr() { return std::addressof(storage_.value_); }

        const T& value() const { return storage_.value_; }

        T& value() { return storage_.value_; }

        // error: 'T& std2::optional<T>::value()' cannot be overloaded with 'T&
        // std2::optional<T>::value() &'
        // T& value() & { return storage_.value_; }

        // error: 'T& std2::optional<T>::value()' cannot be overloaded with 'T&&
        // std2::optional<T>::value() &&'
        // T&& value() && { return std::move(storage_.value_); }

        //  error: 'T& std2::optional<T>::value()' cannot be overloaded with 'constexpr const T&
        //  std2::optional<T>::value() const &'
        // constexpr const T& value() const& { return storage_.value_; }

        template <class... Args>
        void initialize(Args&&... args) noexcept {
            // It depends on its initial value, which is given in the statement of declaration&definition.
            assert(!init_);  // when its true, skip the assert

            // replacement new operator
            if constexpr (std::is_aggregate_v<T>)
                ::new (static_cast<void*>(dataptr())) T{std::forward<Args>(args)...};
            else
                ::new (static_cast<void*>(dataptr())) T(std::forward<Args>(args)...);
            init_ = true;
        }

        template <class U>
        auto operator=(U&& v) ->
          typename std::enable_if<std::is_same<typename std::decay<U>::type, T>::value, optional&>::type {
            if (initialized()) {
                value() = std::forward<U>(v);
            } else {
                initialize(std::forward<U>(v));
            }
            return *this;
        }

        void clear() noexcept {
            if (initialized()) {
                // && !std::is_trivially_destructible<T>::value
                // It is not work for A* is trivially destructible too.
                dataptr()->T::~T();
            }
            init_ = false;
        }

        // 20.5.4.2, Destructor

        ~optional() { clear(); }

      private:
        // We'd must define them with initial value if calling it before initializing it.
        bool init_ = false;
        storage_t<T> storage_;
        // or
        // typename std::aligned_storage<sizeof(T), alignof(T)>::type data_;
    };
}  // namespace std2
