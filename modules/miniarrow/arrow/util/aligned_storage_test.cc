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

    struct TestStruct {
        int i;
        char c;
        float f;
    };

    TEST(AlignedStorage, user_struct_can_memcpy) {
        EXPECT_EQ(4, sizeof(int));
        EXPECT_EQ(4, sizeof(float));
        EXPECT_EQ(8, sizeof(double));

        EXPECT_EQ(16, sizeof(TestStruct));
        // https://en.cppreference.com/w/cpp/language/alignof
        // sizeof gives the size of a type (how much memory it takes)
        // whereas alignof gives what how many bytes a type must be aligned to.
        EXPECT_EQ(8, alignof(TestStruct));  // why is 8?

        // using storage_type = internal::AlignedStorage<int*>;
        // EXPECT_TRUE(storage_type::can_memcpy);
        // EXPECT_EQ(8, sizeof(storage_type));
    }

}  // namespace arrow
