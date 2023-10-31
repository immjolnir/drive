#include <gtest/gtest.h>

#include <google/protobuf/repeated_field.h>

#include <vector>

TEST(repeated_field, ctor_with_iterator_params) {
    std::vector<int> v{2, 3, 5, 1, 0, -9};
    EXPECT_EQ(6, v.size());
    google::protobuf::RepeatedField<int> repi(v.begin(), v.end());
    v.clear();                  // Clear the vector
    EXPECT_EQ(6, repi.size());  // Still be 6 for the ctor will copy all elements through iterator.
}

TEST(repeated_field, ctor_with_iterator_params_better_performance) {
    /**
     * How to initialize a repeated element in protocol buffer?
     * https://stackoverflow.com/questions/33193372/how-to-initialize-a-repeated-element-in-protocol-buffer
     *
     * For instance, you could call a.mutable_w()->Reserve(n) to pre-allocate space in advance, to avoid the usual
     * strategy of re-allocating every time the size doubles. You can also use Resize(n) followed by mutable_data() to
     * get direct access to the underlying buffer, but this probably won't provide much speed benefit over Reserve(n)
     * followed by your loop.
     */

    std::vector<int> v{2, 3, 5, 1, 0, -9};
    EXPECT_EQ(6, v.size());
    google::protobuf::RepeatedField<int> repi;

    // Key: It will allocate a big block to store the number of v.size() elements.
    // Just like the usage of std::vector.
    repi.Reserve(v.size());
    repi.Add(v.begin(), v.end());

    v.clear();                  // Clear the std::vector
    EXPECT_EQ(6, repi.size());  // Still be 6 for the ctor will copy all elements through iterator.
}

TEST(repeated_field, general_methods) {
    google::protobuf::RepeatedField<int> repi;
    repi.Add(1);
    EXPECT_EQ(1, repi.size());
    EXPECT_EQ(1, repi[0]);

    int* _0i = repi.Mutable(0);  // index= 0
    *_0i = 9;
    EXPECT_EQ(1, repi.size());
    EXPECT_EQ(9, repi[0]);

    repi[0] = 10;  // operator [] is overrided for read and write.
    EXPECT_EQ(1, repi.size());
    EXPECT_EQ(10, repi[0]);

    repi.Set(0, 1);
    EXPECT_EQ(1, repi.size());
    EXPECT_EQ(1, repi[0]);

    repi.RemoveLast();
    EXPECT_TRUE(repi.empty());
    EXPECT_EQ(0, repi.size());
}