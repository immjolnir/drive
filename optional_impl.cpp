// https://www.geeksforgeeks.org/structures-unions-and-enumerations-in-cpp/
//
#include <iostream>
#include <type_traits>
#include <string>


// The size of the union is the size of its largest field (or larger if alignment so requires, for example on a SPARC machine a union contains a double and a char [17] so its size is likely to be 24 because it needs 64-bit alignment). Unions cannot have a destructor.
//
//
union Foo {
    double d;
    char arr[17];
};
static_assert(sizeof(Foo) == 24, "Its size is 24B");

union alignas(64) Bar{
    double d;
    char arr[17];
};
static_assert(sizeof(Bar) == 64, "Its size is 24B");

constexpr struct trivial_init_t{} trivial_init{};
static_assert(sizeof(trivial_init_t) == 1, "The size of an empty struct is 1 B");

constexpr struct in_place_t{} in_place{};

template <class T>
union storage_t
{
  unsigned char dummy_;
  T value_;

  constexpr storage_t( trivial_init_t ) noexcept : dummy_() {};

  template <class... Args>
  constexpr storage_t( Args&&... args ) : value_(std::forward<Args>(args)...) {}

  ~storage_t(){}
};
// use  std::aligned_union_t<1, G, H> storage;


template <class T>
struct optional_base
{
    bool init_;
    storage_t<T> storage_;

    constexpr optional_base() noexcept : init_(false), storage_(trivial_init) {};

    explicit constexpr optional_base(const T& v) : init_(true), storage_(v) {}

    explicit constexpr optional_base(T&& v) : init_(true), storage_(constexpr_move(v)) {}

    template <class... Args>
    explicit optional_base(in_place_t, Args&&... args): init_(true), storage_(std::forward<Args>(args)...) {}

    ~optional_base() { if (init_) storage_.value_.T::~T(); }
};

int main() {
    std::cout << sizeof(storage_t<int>) << std::endl;
    std::cout << sizeof(storage_t<std::string>) << std::endl;
    std::cout << sizeof(std::string) << std::endl;

    std::cout << sizeof(storage_t<trivial_init_t>) << std::endl;
    std::cout << sizeof(storage_t<in_place_t>) << std::endl;

    std::cout << sizeof(Foo) << std::endl; // 24
}
