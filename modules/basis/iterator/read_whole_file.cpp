#include <gtest/gtest.h>

#include <iterator>
#include <sstream>

// https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator
// std::istreambuf_iterator is a single-pass input iterator that reads successive characters from the
// std::basic_streambuf object for which it was constructed.
TEST(iterator, read_whole_file) {
    // typical use case: an input stream represented as a pair
    // of iterators
    std::istringstream in{"Hello, world"};
    std::istreambuf_iterator<char> it{in}, end;

    /// Constructs the string with the contents of the range [first, last).
    // Possible implementation:
    /// while (__beg != __end && __len < __capacity) {
    ///     _M_data()[__len++] = *__beg;
    ///     ++__beg;
    /// }
    std::string ss{it, end};
    EXPECT_EQ(12, ss.size());
    EXPECT_EQ("Hello, world", ss);

    ///
    /// Sample usage
    /// ```
    /// std::ifstream in("file.txt");
    /// std::istreambuf_iterator<char> it{in}, end;
    /// std::string text{it, end};
    /// ```
}

TEST(iterator, istreambuf_iterator) {
    // demonstration of the single-pass nature
    std::istringstream s{"abc"};
    std::istreambuf_iterator<char> i1{s}, i2{s};
    EXPECT_EQ('a', *i1);
    EXPECT_EQ('a', *i2);

    ++i1;
    EXPECT_EQ('b', *i1);
    EXPECT_EQ('b', *i2);  // after incrementing i1, but not i2. why they have same value?

    ++i2;
    // after incrementing i2, but not i1. why they have same value?
    EXPECT_EQ('c', *i1);
    EXPECT_EQ('c', *i2);
    /*
    From its implementation, we can know both `++i1` and `++i2` are working on the same objct.
    .-----------.
    | i1:_M_sbuf | --------
    '-----------'         \
                          \
                        .-----------.
                        | s:_M_sbuf |
                        '-----------'
                          /
    .-----------.        /
    | i2:_M_sbuf | -------
    '-----------'
    */
}
