#include <gtest/gtest.h>

#include <atomic>
#include <cstdint>
#include <cstring>  // memcpy

/*
https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange

bool compare_exchange_strong( T& expected, T desired,
                              std::memory_order success,
                              std::memory_order failure ) noexcept;

Atomically compares the object representation (until C++20)value representation (since C++20) of *this with that of
expected. If those are bitwise-equal, replaces the former with desired (performs read-modify-write operation).
Otherwise, loads the actual value stored in *this into expected (performs load operation).


- Return value
  true if the referenced object was successfully changed, false otherwise.
*/
TEST(atomic, quick_start) {
    std::atomic<int> ai;
    int tst_val = 4;
    int new_val = 5;
    bool exchanged = false;

    ai = 3;

    // tst_val != ai   ==>  tst_val is modified
    exchanged = ai.compare_exchange_strong(tst_val, new_val);
    EXPECT_FALSE(exchanged);  // the ai is not changed.
    // those are not bitwise-equal, loads the actual value stored in *this into expected (performs load operation).
    EXPECT_EQ(3, ai);
    EXPECT_EQ(3, tst_val);  // loading the *this into expected.
    EXPECT_EQ(5, new_val);

    // tst_val == ai   ==>  ai is modified
    exchanged = ai.compare_exchange_strong(tst_val, new_val);
    // those are bitwise-equal, replaces the former with __desired__ (performs read-modify-write operation).
    // The former means the *this.
    EXPECT_TRUE(exchanged);
    EXPECT_EQ(5, ai);
    // replace the tst_val with new_val.
    EXPECT_EQ(3, tst_val);
    EXPECT_EQ(5, new_val);
}

/*
Base on the above test case, we can show the its possible implementation
*/

namespace possible_impl
{
    template <typename T>
    class Atomic {
      public:
        // typedef T* __pointer_type;
        typedef T __pointer_type;

        Atomic() noexcept = default;
        ~Atomic() noexcept = default;
        Atomic(const Atomic&) = delete;
        Atomic& operator=(const Atomic&) = delete;
        Atomic& operator=(const Atomic&) volatile = delete;

        constexpr Atomic(__pointer_type data_) noexcept : data(data_) {}

        operator __pointer_type() const noexcept { return data; }

        operator __pointer_type() const volatile noexcept { return data; }

        __pointer_type operator=(__pointer_type __p) noexcept {
            data = __p;
            return data;
        }

        bool compare_exchange_strong(T& expected, T desired) {
            /* Here just simplify it.

            https://stackoverflow.com/questions/26724418/comparison-semantics-with-stdatomic-types
            Note: For example, the effect of atomic_compare_exchange_strong is
            if (memcmp(object, expected, sizeof(*object)) == 0)
                memcpy(object, &desired, sizeof(*object));
            else
                memcpy(expected, object, sizeof(*object));

            Note: The memcpy and memcmp semantics of the compare-and-exchange operations may result in failed
            comparisons for values that compare equal with operator== if the underlying type has padding bits, trap
            bits, or alternate representations of the same value.
            */
            if (this->data == expected) {  // bitwise-equal
                // this->data = &desired;
                this->data = desired;
                return true;
            } else {
                expected = *this;
                return false;
            }
        }

        const __pointer_type operator()() const { return data; }

      private:
        __pointer_type data;
    };

    using AtomicInt = Atomic<int>;

    TEST(atomic, possible_implementation) {
        AtomicInt ai;
        int tst_val = 4;
        int new_val = 5;
        bool exchanged = false;

        ai = 3;

        // tst_val != ai   ==>  tst_val is modified
        exchanged = ai.compare_exchange_strong(tst_val, new_val);
        EXPECT_FALSE(exchanged);  // the ai is not changed.
        // those are not bitwise-equal, loads the actual value stored in *this into expected (performs load operation).
        EXPECT_EQ(3, ai);
        EXPECT_EQ(3, tst_val);  // loading the *this into expected.
        EXPECT_EQ(5, new_val);

        // tst_val == ai   ==>  ai is modified
        exchanged = ai.compare_exchange_strong(tst_val, new_val);
        // those are bitwise-equal, replaces the former with __desired__ (performs read-modify-write operation).
        // The former means the *this.
        EXPECT_TRUE(exchanged);
        EXPECT_EQ(5, ai);
        // replace the tst_val with new_val.
        EXPECT_EQ(3, tst_val);
        EXPECT_EQ(5, new_val);
    }
}  // namespace possible_impl

struct Foo {
    std::uint32_t a;
    std::uint64_t b;
};

TEST(atomic, type_traits) {
    EXPECT_TRUE(std::is_trivial_v<Foo>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Foo>);
    EXPECT_TRUE(std::is_standard_layout_v<Foo>);
    EXPECT_TRUE(std::is_aggregate_v<Foo>);
}

TEST(atomic, assignement) {
    Foo expected{}, desired{2, 2};
    EXPECT_EQ(0, expected.a);
    EXPECT_EQ(0, expected.b);

    expected = desired;
    EXPECT_EQ(2, expected.a);
    EXPECT_EQ(2, expected.b);
}

TEST(atomic, memcpy) {
    Foo expected{}, desired{2, 2};
    EXPECT_EQ(0, expected.a);
    EXPECT_EQ(0, expected.b);

    // void* memcpy( void* dest, const void* src, std::size_t count );
    std::memcpy(&expected, &desired, sizeof(Foo));
    EXPECT_EQ(2, expected.a);
    EXPECT_EQ(2, expected.b);
}

TEST(atomic, memcmp) {
    Foo expected{}, desired{};
    EXPECT_EQ(0, expected.a);
    EXPECT_EQ(0, expected.b);

    // int memcmp( const void* lhs, const void* rhs, std::size_t count );
    // Return value: ​0​ if all count bytes of lhs and rhs are equal
    int res = std::memcmp(&expected, &desired, sizeof(Foo));
    EXPECT_EQ(0, res);
}

TEST(atomic, compare_exchange_strong_bad_case) {
    Foo init{1, 1}, expected{}, desired{2, 2};

    /*
        constexpr atomic(T desired) noexcept;
        2) Initializes the underlying object with desired. The initialization is not atomic.
    */
    std::atomic<Foo> atom(init);  // Construct atom with the init(default) value

    // *this != expected ==> expected is modified with *this
    // 1. it performs load operation: loading the *this into expected.
    EXPECT_FALSE(atom.compare_exchange_strong(expected, desired));
    EXPECT_EQ(1, init.a);
    EXPECT_EQ(1, init.b);

    EXPECT_EQ(1, expected.a);  // changed
    EXPECT_EQ(1, expected.b);  // changed

    EXPECT_EQ(2, desired.a);
    EXPECT_EQ(2, desired.b);
    {
        Foo underlying_v = atom.load();
        EXPECT_EQ(1, underlying_v.a);
        EXPECT_EQ(1, underlying_v.b);
    }

    // Bad case
    // https://stackoverflow.com/questions/48947428/atomic-variable-with-padding-compiler-bug
    // But I cannot reproduce that bug mentioned above.
    //
    // *this == expected ==> *this is modified with desired.
    // 2. replaces the former(*this) with desired (performs read-modify-write operation).
    EXPECT_TRUE(atom.compare_exchange_strong(expected, desired));
    EXPECT_EQ(1, init.a);
    EXPECT_EQ(1, init.b);

    EXPECT_EQ(1, expected.a);
    EXPECT_EQ(1, expected.b);

    EXPECT_EQ(2, desired.a);
    EXPECT_EQ(2, desired.b);
    {
        Foo underlying_v = atom.load();
        EXPECT_EQ(2, underlying_v.a);  // changed
        EXPECT_EQ(2, underlying_v.b);  // changed
    }
}
