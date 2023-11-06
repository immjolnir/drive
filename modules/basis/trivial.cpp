#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <numeric>
#include <string>
#include <type_traits>

struct Foo {
    int i;
    char c;
};

struct Foo1 {
    int i = 0;
    char c = 'a';
};

struct Foo2 {
    Foo2(int i_ = 1, char c_ = 'a') : i(i_), c(c_) {}

    int i;
    char c;
};

struct Foo3 {
    Foo3(int i_, char c_) : i(i_), c(c_) {}

    int i;
    char c;
};

struct Foo4 {
    Foo4(int i_, char c_) : i(i_), c(c_) {}

    int i;
    char c;

    ~Foo4() {}
};

struct alignas(8) Bar {
    int i;
    char c;
};

TEST(trivial, trivial_vs_trivially_copyable) {
    EXPECT_TRUE(std::is_trivial<int>::value);
    EXPECT_TRUE(std::is_trivially_copyable<int>::value);

    EXPECT_TRUE(std::is_trivial<Foo>::value);
    EXPECT_TRUE(std::is_trivially_copyable<Foo>::value);

    EXPECT_FALSE(std::is_trivial<Foo1>::value);
    EXPECT_TRUE(std::is_trivially_copyable<Foo1>::value);

    EXPECT_FALSE(std::is_trivial<Foo2>::value);
    EXPECT_TRUE(std::is_trivially_copyable<Foo2>::value);

    EXPECT_FALSE(std::is_trivial<Foo3>::value);
    EXPECT_TRUE(std::is_trivially_copyable<Foo3>::value);

    EXPECT_FALSE(std::is_trivial<Foo4>::value);
    EXPECT_FALSE(std::is_trivially_copyable<Foo4>::value);

    EXPECT_TRUE(std::is_trivial<Bar>::value);
    EXPECT_TRUE(std::is_trivially_copyable<Bar>::value);

    EXPECT_FALSE(std::is_trivial<std::string>::value);
    EXPECT_FALSE(std::is_trivially_copyable<std::string>::value);
}

template <typename T>
struct CanMemcpy {
    // which is the suitable method: std::is_trivial or std::is_trivially_copyable?
    static constexpr bool value = std::is_trivial<T>::value;
    // is_trivially_copyable
    // Objects of trivially-copyable types that are not potentially-overlapping subobjects are the only C++ objects that
    // may be safely copied with std::memcpy or serialized to/from binary files with std::ofstream::write() /
    // std::ifstream::read().
};

TEST(trivial, can_memcpy) {
    EXPECT_TRUE(CanMemcpy<int>::value);

    EXPECT_TRUE(CanMemcpy<Foo>::value);
    EXPECT_FALSE(CanMemcpy<Foo1>::value);
    EXPECT_FALSE(CanMemcpy<Foo2>::value);
    EXPECT_FALSE(CanMemcpy<Foo3>::value);

    EXPECT_TRUE(CanMemcpy<Bar>::value);
    EXPECT_FALSE(CanMemcpy<std::string>::value);
}

// trivial type is that they can have mixed access specifiers.
// The result is that a trivial type is not usable in C code.
struct A {
  public:
    int a;
    int b;
    int c;
};

struct B {
  public:
    int a;

  private:
    int b;

  public:
    int c;
};

TEST(trivial, max_access_specifiers) {
    EXPECT_TRUE(std::is_trivial<A>::value);
    EXPECT_TRUE(std::is_trivially_copyable<A>::value);
    EXPECT_TRUE(std::is_standard_layout<A>::value);

    EXPECT_TRUE(std::is_trivial<B>::value);
    EXPECT_TRUE(std::is_trivially_copyable<B>::value);
    EXPECT_FALSE(std::is_standard_layout<B>::value);
}

struct C {
    int& a;
};

TEST(standard_layout, has_reference_member) {
    EXPECT_TRUE(std::is_trivial<C>::value);
    EXPECT_TRUE(std::is_trivially_copyable<C>::value);
    EXPECT_FALSE(std::is_standard_layout<C>::value);
}

struct D {
    // A Base is no aggregate due to the deleted default constructor
    int i;

    D(int a) : i(a) {}
};

struct E {
    int i;

    E(int a) : i(a) {}
};

struct F : D {
    int x;
    E y;
};

TEST(aggregate, aggregate) {
    EXPECT_FALSE(std::is_aggregate_v<D>);
    EXPECT_FALSE(std::is_aggregate_v<E>);
    EXPECT_TRUE(std::is_aggregate_v<F>);
}

// https://en.cppreference.com/w/cpp/types/is_aggregate
// constructs a T at the uninitialized memory pointed to by p using
// list-initialization for aggregates and non-list initialization otherwise
template <class T, class... Args>
T* construct(T* p, Args&&... args) {
    if constexpr (std::is_aggregate_v<T>) {
        // Performing aggregate initialization
        // https://en.cppreference.com/w/cpp/language/aggregate_initialization
        return ::new (static_cast<void*>(p)) T{std::forward<Args>(args)...};
    } else {
        return ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
    }
}

struct G {
    int x, y;
};

struct H {
    H(int x_, const char* data_) : x(x_), data(data_) {}

    int x;
    const char* data;
};

TEST(aggregate, aggregate_storage) {
    EXPECT_TRUE(std::is_aggregate_v<G>);

    EXPECT_FALSE(std::is_aggregate_v<H>);
    EXPECT_FALSE(std::is_trivial<H>::value);
    EXPECT_TRUE(std::is_trivially_copyable<H>::value);  // Still memcopyable

    std::aligned_union_t<1, G, H> storage;
    [[maybe_unused]] G* a = construct(reinterpret_cast<G*>(&storage), 1, 2);
    [[maybe_unused]] H* b = construct(reinterpret_cast<H*>(&storage), 1, "hello");
    EXPECT_EQ(1, a->x);
    EXPECT_EQ(2, a->y);

    EXPECT_EQ(1, b->x);
    EXPECT_EQ("hello", b->data);
}

// memcpy vs std::copy
TEST(aggregate, std_copy) {
    std::vector<int> from_vector(10);
    std::iota(from_vector.begin(), from_vector.end(), 0);
    EXPECT_THAT(from_vector, testing::ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

    std::vector<int> to_vector;
    std::copy(from_vector.begin(), from_vector.end(), std::back_inserter(to_vector));

    EXPECT_THAT(to_vector, testing::ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

    EXPECT_EQ(std::string(from_vector.begin(), from_vector.end()), std::string(to_vector.begin(), to_vector.end()));

    to_vector.clear();
    std::copy_if(from_vector.begin(), from_vector.end(), std::back_inserter(to_vector),
                 [](int x) { return x % 3 == 0; });
    EXPECT_THAT(to_vector, testing::ElementsAre(0, 3, 6, 9));
}