#include <gtest/gtest.h>

#include <string>  // std::getline
#include <vector>

#include <boost/algorithm/string/classification.hpp>  // boost::is_any_of
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>  // boost::algorithm::split
#include <boost/regex.hpp>

#include <sstream>

std::string line("test\ttest1\ttest2");

TEST(string_split, boost_alg_split) {
    // https://stackoverflow.com/questions/5734304/c-boost-split-string
    std::vector<std::string> strs;
    boost::split(strs, line, boost::is_any_of("\t"));

    ASSERT_EQ(3, strs.size());

    for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); ++it) {
        std::cout << *it << std::endl;
    }

    ASSERT_EQ("test", strs[0]);
    ASSERT_EQ("test1", strs[1]);
    ASSERT_EQ("test2", strs[2]);
}

TEST(string_split, boost_alg_split_regex) {
    // https://stackoverflow.com/questions/5734304/c-boost-split-string
    std::vector<std::string> strs;
    boost::algorithm::split_regex(strs, line, boost::regex("\t"));

    ASSERT_EQ(3, strs.size());

    for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); ++it) {
        std::cout << *it << std::endl;
    }

    ASSERT_EQ("test", strs[0]);
    ASSERT_EQ("test1", strs[1]);
    ASSERT_EQ("test2", strs[2]);
}

TEST(string_split, stringstream_getline) {
    // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    // https://en.cppreference.com/w/cpp/string/basic_string/getline
    // getline( std::basic_istream<CharT, Traits>& input, std::basic_string<CharT, Traits, Allocator>& str, CharT delim
    // );
    // Limits: The delim must be char type.
    std::vector<std::string> strs;
    std::stringstream ss(line);

    std::string item;
    while (std::getline(ss, item, '\t')) {
        strs.push_back(item);
    }

    ASSERT_EQ(3, strs.size());
    ASSERT_EQ("test", strs[0]);
    ASSERT_EQ("test1", strs[1]);
    ASSERT_EQ("test2", strs[2]);
}