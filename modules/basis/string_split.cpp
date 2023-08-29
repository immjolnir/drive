#include <gtest/gtest.h>

#include <string>  // std::getline
#include <vector>

#include <boost/algorithm/string/classification.hpp>  // boost::is_any_of
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>  // boost::algorithm::split
#include <boost/regex.hpp>

#include <sstream>

/* 6 Methods to Split a String in C++
Here is the list of those methods which you can use to split a string into words using your own delimiter function:

- Using stringstream API of C++

- Using strtok() Function

- Using Custom split() Function

- Using std::getline() Function

- Using find(), substr() and erase() Functions

- Boost'split
*/

TEST(string_split, stringstream) {
    // https://stackoverflow.com/questions/4533652/how-to-split-string-using-istringstream-with-other-delimiter-than-whitespace
    /*
    Using stringstream API of C++

    We use cin stream to take input from the user, similarly, we first initialize the stringstream's object and take the
    input in it using "<<" operator which allows it to read a string as a stream of words.

    The most commonly used stringstream operators are as follows:
        Operator<<: pushes a string object into the stream.
        Operator>>: extracts a word from the stream.

    Note: Tokenizing a string means splitting it with respect to a delimiter.
    */
    // https://favtutor.com/blogs/split-string-cpp
    std::string s = "I love to read articles on Favtutor.";
    // Takes only space separated C++ strings.
    std::stringstream ss(s);
    std::vector<std::string> strs;
    std::string word;
    while (ss >> word) {  // Extract word from the stream.
        strs.emplace_back(word);
    }
    EXPECT_EQ(7, strs.size());
    EXPECT_EQ("I", strs[0]);
    EXPECT_EQ("articles", strs[4]);
}

std::string line("test\ttest1\ttest2");

TEST(string_split, strtok) {
    std::vector<std::string> strs;

    // Pointer to point the word returned by the strtok() function.
    char* p;
    // We know that both string::c_str or string::data functions returns const char*. To get a non-const version, we can
    // use the const_cast operator, which removes the const attribute from a class. This works in constant time as no
    // copying is involved.
    // char* s = const_cast<char*>(line.c_str());
    char* s = strcpy(new char[line.length() + 1], line.c_str());
    p = strtok(s, "\t");
    while (p != NULL) {
        strs.emplace_back(p);
        p = strtok(NULL, "\t");
    }

    ASSERT_EQ(3, strs.size());

    for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); ++it) {
        std::cout << *it << std::endl;
    }

    ASSERT_EQ("test", strs[0]);
    ASSERT_EQ("test1", strs[1]);
    ASSERT_EQ("test2", strs[2]);
}

// Create custom split() function.
void customSplit(const std::string& str, char separator, std::vector<std::string>& tokens) {
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {
        // If we reached the end of the word or the end of the input.
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            tokens.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
}

TEST(string_split, custom_split) {
    std::vector<std::string> strs;
    customSplit(line, '\t', strs);

    ASSERT_EQ(3, strs.size());
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

void find_str(std::string s, const std::string& del, std::vector<std::string>& res) {
    // Use find function to find 1st position of delimiter.
    int end = s.find(del);
    while (end != -1) {  // Loop until no delimiter is left in the string.
        res.emplace_back(s.substr(0, end));
        s.erase(s.begin(), s.begin() + end + 1);
        end = s.find(del);
    }
    res.emplace_back(s.substr(0, end));
}

TEST(string_split, find_stbstr_erase) {
    /*
    Using find(), substr() and erase() Functions
    In this method, we will use the find(), substr(), and erase() function to split the given string using our
    delimiter. Syntax: string substr (size_t position, size_t length); Note: The substr() returns a string object and
    size_t is an unsigned integer type. Below is the C++ program implementation :
    */
    std::vector<std::string> strs;
    find_str(line, "\t", strs);
    ASSERT_EQ(3, strs.size());
    ASSERT_EQ("test", strs[0]);
    ASSERT_EQ("test1", strs[1]);
    ASSERT_EQ("test2", strs[2]);
}

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
