#include <sstream>

#include <gtest/gtest.h>
using namespace std;

// clang-format off
/**
Redis source contains many linux console codes: `man console_codes`

Escape Sequence in C,
See https://www.geeksforgeeks.org/escape-sequence-in-c/

Escape Sequence List
The table below lists some common escape sequences in C language.

Escape Sequence	Name	    Description
\a	    Alarm or Beep	    It is used to generate a bell sound in the C program.
\b	    Backspace	        It is used to move the cursor backward.
\f	    Form Feed	        It is used to move the cursor to the start of the next logical page.
\n	    New Line	        It moves the cursor to the start of the next line.
\r	    Carriage Return	    It moves the cursor to the start of the current line.
\t	    Horizontal Tab	    It inserts some whitespace to the left of the cursor and moves the cursor accordingly.
\v	    Vertical Tab	    It is used to insert vertical space.
\\	    Backlash	        Use to insert backslash character.
\’	    Single Quote	    It is used to display a single quotation mark.
\”	    Double Quote	    It is used to display double quotation marks.
\?	    Question Mark	    It is used to double quotation marks.
\ooo	Octal Number	    It is used to represent an octal number.
\xhh	Hexadecimal Number	It represents the hexadecimal number.
\0	    NULL	            It represents the NLL character.


Out of all these escape sequences, \n and \0 are used the most. In fact, escape sequences like \f, \a,  are not even used by programmers nowadays.
*/
// clang-format on

TEST(sstream, remove_last_character) {
    std::stringstream ss;
    // it doest have the member: resize and size.
    // ss.resize(ss.size() - 1);
    ss << "Hello123";
    // Expected, Actual
    ASSERT_EQ("Hello123", ss.str());

    ss << "\b";
    ASSERT_EQ("Hello123\b", ss.str());

    ss << "a";
    // 在 console 上的效果是: Hello123a. 只能作用在 console 上，如果用在 string， 会被当作一个字符.
    ASSERT_EQ("Hello123\ba", ss.str());

    // https://stackoverflow.com/questions/4546021/remove-char-from-stringstream-and-append-some-data
    ss.seekp(-1, std::ios_base::end);
    ss << "x";
    ASSERT_EQ("Hello123\bx", ss.str());

    ss.seekp(-1, std::ios_base::end);
    ss << '\0';  // null character
    string after = ss.str();
    ASSERT_EQ("Hello123\b", after);
}

TEST(sstream, remove_last_character_with_null_character) {
    std::stringstream ss;
    ss << "Hello123";

    ss.seekp(-1, std::ios_base::end);
    ss << '\0';  // null character

    string after;
    ss >> after;
    ASSERT_EQ("Hello12", after); // 这个还在sstream里，只是在console里没有而已。
}