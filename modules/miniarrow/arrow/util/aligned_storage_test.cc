#include <gtest/gtest.h>

#include "arrow/util/aligned_storage.h"

namespace arrow
{

    TEST(AlignedStorage, int_can_memcpy) {
        using storage_type = internal::AlignedStorage<int>;
        EXPECT_TRUE(storage_type::can_memcpy);
        EXPECT_EQ(4, sizeof(storage_type));
    }

    TEST(AlignedStorage, double_can_memcpy) {
        using storage_type = internal::AlignedStorage<double>;
        EXPECT_TRUE(storage_type::can_memcpy);
        EXPECT_EQ(8, sizeof(storage_type));
    }

    TEST(AlignedStorage, int_ptr_can_memcpy) {
        using storage_type = internal::AlignedStorage<int*>;
        EXPECT_TRUE(storage_type::can_memcpy);
        EXPECT_EQ(8, sizeof(storage_type));
    }

    struct Foo {
        int i;
        char c;
        float f;

        void f0() {}
    };

    TEST(AlignedStorage, user_struct_can_memcpy) {
        EXPECT_EQ(4, sizeof(int));
        EXPECT_EQ(4, sizeof(float));
        EXPECT_EQ(8, sizeof(double));

        EXPECT_EQ(12, sizeof(Foo));
        // https://en.cppreference.com/w/cpp/language/alignof
        // sizeof gives the size of a type (how much memory it takes)
        // whereas alignof gives what how many bytes a type must be aligned to.
        EXPECT_EQ(4, alignof(Foo));

        using storage_type = internal::AlignedStorage<Foo>;
        EXPECT_TRUE(storage_type::can_memcpy);
        EXPECT_EQ(12, sizeof(storage_type));
    }

    struct Bar {
        int i;
        char c;
        float f;
        Bar() = default;

        Bar(int i_) : i(i_) {}

        Bar(int i_, char c_) : i(i_), c(c_) {}

        Bar(int i_, char c_, float f_) : i(i_), c(c_), f(f_) {}

        void f0() {}
    };

    TEST(AlignedStorage, user_struct_cannot_memcpy) {
        EXPECT_EQ(12, sizeof(Bar));
        // https://en.cppreference.com/w/cpp/language/alignof
        // sizeof gives the size of a type (how much memory it takes)
        // whereas alignof gives what how many bytes a type must be aligned to.
        EXPECT_EQ(4, alignof(Bar));

        using storage_type = internal::AlignedStorage<Bar>;
        EXPECT_TRUE(storage_type::can_memcpy);
        EXPECT_EQ(12, sizeof(storage_type));
    }

    struct Baz {
        int i;
        char c;
        float f;

        void f0() {}

        virtual void func() {}
    };

    TEST(AlignedStorage, user_struct_cannot_memcpy_baz) {
        EXPECT_EQ(24, sizeof(Baz));  // why its size become 24(doubled)?
        // https://en.cppreference.com/w/cpp/language/alignof
        // sizeof gives the size of a type (how much memory it takes)
        // whereas alignof gives what how many bytes a type must be aligned to.
        EXPECT_EQ(8, alignof(Baz));

        using storage_type = internal::AlignedStorage<Baz>;
        EXPECT_FALSE(storage_type::can_memcpy);  // Why it's false?
        EXPECT_EQ(24, sizeof(storage_type));
    }

}  // namespace arrow
