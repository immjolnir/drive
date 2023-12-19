#include <gtest/gtest.h>

#include <boost/algorithm/string.hpp>

// https://theboostcpplibraries.com/boost.stringalgorithms

// #include <boost/algorithm/string/case_conv.hpp>

/*
    // pull names to the boost namespace
    using algorithm::to_lower;
    using algorithm::to_lower_copy;
    using algorithm::to_upper;
    using algorithm::to_upper_copy;
*/

TEST(algorithm_string, case_conv) {
    std::string msg = "Hello";
    EXPECT_EQ("hello", boost::to_lower_copy(msg));
    EXPECT_EQ("HELLO", boost::to_upper_copy(msg));
    EXPECT_EQ("Hello", msg);  // Not changed

    boost::to_lower(msg);
    EXPECT_EQ("hello", msg);

    boost::to_upper(msg);
    EXPECT_EQ("HELLO", msg);
}

TEST(algorithm_string, erase) {
    std::string s = "Helloe";
    EXPECT_EQ("Hlloe", boost::erase_first_copy(s, "e"));  // remove only the first occurrence
    EXPECT_EQ("Hello", boost::erase_nth_copy(s, "e", 1));
    EXPECT_EQ("Hello", boost::erase_last_copy(s, "e"));
    EXPECT_EQ("Hllo", boost::erase_all_copy(s, "e"));  // remove all occurrences
    EXPECT_EQ("lloe", boost::erase_head_copy(s, 2));
    EXPECT_EQ("He", boost::erase_tail_copy(s, 4));
}

TEST(algorithm_string, find) {
    std::string s = "Boost C++ Libraries";
    boost::iterator_range<std::string::iterator> r = boost::find_first(s, "C++");
    /*
    //! iterator_range output operator
            Output the range to an ostream. Elements are outputed
            in a sequence without separators.
    template <typename IteratorT>
    inline std::ostream& operator<<(std::ostream& Os, const iterator_range<IteratorT>& r) {
        std::copy(r.begin(), r.end(), std::ostream_iterator<char>(Os));
        return Os;
    }
    */
    std::cout << r << std::endl;
    EXPECT_EQ("C++", std::string(r.begin(), r.end()));  // it doesn't have dereference operator.

    r = boost::find_first(s, "xyz");
    EXPECT_TRUE(r.empty());

    // Functions such as boost::algorithm::find_first(), boost::algorithm::find_last(), boost::algorithm::find_nth(),
    // boost::algorithm::find_head() and boost::algorithm::find_tail() are available to find strings within strings.
}