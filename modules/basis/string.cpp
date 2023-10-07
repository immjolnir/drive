#include <string>

#include <gtest/gtest.h>
using namespace std;

TEST(string, deep_copy) {
    // The std::string is deep copy.
    // https://stackoverflow.com/questions/8467049/c-stl-string-copy-constructor-always-a-deep-copy
    std::string s0 = "abc";
    std::string s1 = s0;
    EXPECT_EQ(s0, s1);

    s0[0] = 'x';
    EXPECT_EQ("xbc", s0);
    EXPECT_EQ("abc", s1);
}

TEST(string, remove_last_character_with_pop_back) {
    // use the pop_back() method to remove the last character of a string in C++
    //
    // The pop_back() method of the string class pops up the last character from the string and reduces the length of
    // the string by one. The method does not return anything and also does not accept any arguments.

    string str("world1");
    str.pop_back();
    EXPECT_EQ("world", str);
}

TEST(string, remove_last_character_with_substr) {
    /**
     * string substr (size_t pos = 0, size_t len = npos) const;
     * The first argument represents the starting index (we have 0-indexed strings in C++) and the second argument
     * represents the length of the substring.
     */
    /**
     *  @brief  Get a substring.
     *  @param __pos  Index of first character (default 0).
     *  @param __n  Number of characters in substring (default remainder).
     *  @return  The new string.
     *  @throw  std::out_of_range  If __pos > size().
     *
     *  Construct and return a new string using the @a __n
     *  characters starting at @a __pos.  If the string is too
     *  short, use the remainder of the characters.  If @a __pos is
     *  beyond the end of the string, out_of_range is thrown.
     */
    string str("world1");
    str = str.substr(0, str.size() - 1);  // return a new string
    EXPECT_EQ("world", str);
}

/**
 *  @brief  Resizes the %string to the specified number of characters.
 *  @param  __n  Number of characters the %string should contain.
 *
 *  This function will resize the %string to the specified length.  If
 *  the new size is smaller than the %string's current size the %string
 *  is truncated, otherwise the %string is extended and new characters
 *  are default-constructed.  For basic types such as char, this means
 *  setting them to 0.
 */
TEST(string, remove_last_character_with_resize) {
    string str("world1");
    // clang-format off
    /*
    gcc/libstdc++-v3/include/bits/basic_string.tcc

    template <typename _CharT, typename _Traits, typename _Alloc>
    void basic_string<_CharT, _Traits, _Alloc>::resize(size_type __n, _CharT __c) {
        const size_type __size = this->size();
        if (__size < __n)
            this->append(__n - __size, __c); // 当__n 大于 capacity 时，会重新分配空间，并把 原来的数据 copy 到新的地址里, 代价更高
        else if (__n < __size)
            this->_M_set_length(__n);  // 当缩小string的size时，代价更小，只需要更新 length 即可
    }
    */
    // clang-format on
    str.resize(str.size() - 1);
    EXPECT_EQ("world", str);
}

/*
This method erases the part of the string and reduces its length.
The erase() method is also the in-place method therefore we do not need to reassign the string.
To remove the last character using the erase() method, we will invoke the erase() method on the original string. Also,
we will pass one less than the size of the original string as the input argument.

The erase() method removes the character to which the iterator is passed.


*/
/*
template <typename _CharT, typename _Traits, typename _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_erase(size_type __pos, size_type __n) {

    const size_type __how_much = length() - __pos - __n;
    if (__how_much && __n) {
        this->_S_move(_M_data() + __pos, _M_data() + __pos + __n, __how_much);
    }
    _M_set_length(length() - __n);
}
*/
/**
 *  @brief  Remove one character.
 *  @param __position  Iterator referencing the character to remove.
 *  @return  iterator referencing same location after removal.
 *
 *  Removes the character at @a __position from this string. The value
 *  of the string doesn't change if an error is thrown.
 *
 *   iterator erase (iterator p);
 *
 */
/*
    gcc/libstdc++-v3/include/bits/basic_string.tcc
static void _S_move(_CharT* __d, const _CharT* __s, size_type __n) {
    if (__n == 1)
        traits_type::assign(*__d, *__s);
    else
        traits_type::move(__d, __s, __n);
}

traits_type 即是 char_traits.

// gcc/libstdc++-v3/include/bits/char_traits.h
template <typename _CharT>
_GLIBCXX20_CONSTEXPR typename char_traits<_CharT>::char_type* char_traits<_CharT>::move(char_type* __s1,
                                                                                        const char_type* __s2,
                                                                                        std::size_t __n) {
    if (__n == 0) return __s1;
    return static_cast<_CharT*>(__builtin_memmove(__s1, __s2, __n * sizeof(char_type)));
}
*/
// Suppliment: https://en.cppreference.com/w/c/string/byte/memmove
TEST(string, remove_last_character_with_erase) {
    string str("world1");
    str.erase(str.size() - 1);  // 虽然是insplace, 但还是涉及到了重新copy
    EXPECT_EQ("world", str);
}

/*
Character arrays are a classic way to store text data.We can store a sequence of characters into a character array.To
mark the end of the text data or a character string, the last element in a character array is the null character ‘\0’.

Therefore, to remove the last character from a character array, we can effectively move the null character to one place
left.In this way, our character array will be terminated with one character less than the original character array.

So to remove the last character from the character array, we will perform the following steps.

- We will iterate the array until we find the null character ‘\0’ to get the size of the array.

- We will put the null character ‘\0’ at the second last place.

You should note that the last place in the character array is for the null character so the last readable character is
actually at the second-last place
*/
TEST(string, remove_last_character_with_null_character) {
    string str("world1");
    str[str.size() - 1] = '\0';
    EXPECT_EQ("world", str);  // Actual: "World\o" 这是因为没有修改size的原因， string 跟 raw char array 还是有差别的。
}

TEST(char_array, remove_last_character_with_0) {
    // char arr[256] = {'\0'};
    // memset， memcpy
    char arr[] = {'w', 'o', 'r', 'l', 'd', '1'};
    EXPECT_EQ(6, strlen(arr));

    arr[5] = '\0';  // empty the last element.
    EXPECT_EQ(5, strlen(arr));
}