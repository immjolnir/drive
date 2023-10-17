#include <gtest/gtest.h>

#include "std2_optional.h"

#include <array>
#include <vector>  // std::vector<int> is not aggregate type.

struct A {
    int x, y;
};

struct B {
    int i = 0;
    A* a = nullptr;

    B() { std::cout << "B()" << std::endl; }

    B(int i_, int j, int k) : i(i_) {
        std::cout << "B(int, int, int)" << std::endl;
        a = new A{j, k};
    }

    ~B() {
        std::cout << "~B()" << std::endl;
        if (a) {
            delete a;
            a = nullptr;
        }
    }
};

namespace std2
{
    TEST(std2_optional, union_size) {
        EXPECT_EQ(4, sizeof(storage_t<int>));

        EXPECT_EQ(32, sizeof(std::string));
        EXPECT_EQ(32, sizeof(storage_t<std::string>));

        EXPECT_EQ(1, sizeof(storage_t<trivial_init_t>));
        EXPECT_EQ(1, sizeof(storage_t<in_place_t>));
    }

    TEST(std2_optional, initialized) {
        optional<int> i;
        EXPECT_FALSE(i.initialized());

        optional<int> j(2);
        EXPECT_TRUE(j.initialized());
        EXPECT_EQ(2, j.value());

        j.clear();
        EXPECT_FALSE(j.initialized());
    }

    TEST(std2_optional, traits) {
        EXPECT_TRUE(std::is_aggregate_v<A>);
        EXPECT_TRUE(std::is_trivially_destructible_v<A>);
        EXPECT_TRUE(std::is_trivially_destructible_v<A*>);  // also trivially

        EXPECT_FALSE(std::is_aggregate_v<B>);
        EXPECT_FALSE(std::is_trivially_destructible_v<B>);
        EXPECT_TRUE(std::is_trivially_destructible_v<B*>);  // Why it is true?
    }

    TEST(std2_optional, initializer_list) {
        optional<A> vec(1, 2);

        EXPECT_TRUE(vec.initialized());
        const A& val = vec.value();
        EXPECT_EQ(1, val.x);
        EXPECT_EQ(2, val.y);
    }

    TEST(std2_optional, destructible) {
        optional<B> vec(1, 2, 3);

        EXPECT_TRUE(vec.initialized());
        const B& val = vec.value();
        EXPECT_EQ(1, val.i);
        EXPECT_EQ(2, val.a->x);
        EXPECT_EQ(3, val.a->y);
    }

}  // namespace std2