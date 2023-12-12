#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <functional>
#include <random>
#include <string>

/*
Question1: Difference between size() and lenght()
https://stackoverflow.com/questions/905479/stdstring-length-and-size-member-functions


these are just synonyms.
- size() is there to be consistent with other STL containers (like vector, map, etc.) and

- length() is to be consistent with most peoples' intuitive notion of character strings. People usually talk about a
word, sentence or paragraph's length, not its size, so length() is there to make things more readable.


Question2: Difference between capacity() and size()?
https://stackoverflow.com/questions/18996317/difference-between-size-and-capacity-in-c-string

- capacity is the maximum number of characters that the string can currently hold without having to grow.
- size is how many characters actually exist in the string.

The reason they're separate concepts is that allocating memory is generally inefficient, so you try to allocate as
rarely as possible by grabbing more memory than you actually need at one time. (Many data structures use a "doubling"
method where, if they hit their capacity of N and need more space, they will allocate 2*N space, to avoid having to
reallocate again any time soon.)

capacity will increase automatically as you use the string and require more space.
You can also manually increase it using the reserve function.
*/
auto generate_random_alphanumeric_string(std::size_t len) -> std::string {
    static constexpr auto chars =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 mt_rand(static_cast<long unsigned int>(seed));
    // auto rng = mt_rand();
    std::uniform_int_distribution<> dist(1, strlen(chars) - 1);
    auto result = std::string(len, '\0');
    std::generate_n(begin(result), len, [&]() { return chars[dist(mt_rand)]; });
    return result;
}

TEST(string, underlying_string_capacity_is_15) {
    std::string str;
    EXPECT_EQ(0, str.size());
    EXPECT_EQ(15, str.capacity());  // Why it is 15?

    str = generate_random_alphanumeric_string(26);
    std::cout << "xxx" << str << std::endl;
    EXPECT_EQ(26, str.length());
    EXPECT_EQ(26, str.capacity());
}

/*
libstdc++-v3/include/bits/basic_string.tcc

template <typename _CharT, typename _Traits, typename _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::reserve(size_type __res) {
    const size_type __capacity = capacity();
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 2968. Inconsistencies between basic_string reserve and
    // vector/unordered_map/unordered_set reserve functions
    // P0966 reserve should not shrink
    if (__res <= __capacity) return;

    pointer __tmp = _M_create(__res, __capacity);
    this->_S_copy(__tmp, _M_data(), length() + 1);
    _M_dispose();
    _M_data(__tmp);
    _M_capacity(__res);
}
*/

TEST(string, underlying_string_capacity_reserve) {
    std::string str;
    str.reserve(32);
    EXPECT_EQ(0, str.length());
    EXPECT_EQ(32, str.capacity());

    str = generate_random_alphanumeric_string(32);  // 32 alphabets.
    EXPECT_EQ(32, str.length());
    EXPECT_EQ(32, str.capacity());
}

/*
template <typename _CharT, typename _Traits, typename _Alloc>
inline basic_string<_CharT, _Traits, _Alloc> operator+(basic_string<_CharT, _Traits, _Alloc>&& __lhs, _CharT __rhs) {
    return std::move(__lhs.append(1, __rhs));
}

*  @brief  Append a string to this string.
*  @param __str  The string to append.
*  @return  Reference to this string.
basic_string& append(const basic_string& __str) { return _M_append(__str._M_data(), __str.size()); }

template <typename _CharT, typename _Traits, typename _Alloc>
basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::
_M_append(const _CharT* __s, size_type __n) {
    const size_type __len = __n + this->size(); // 应该是合并后的长度

    if (__len <= this->capacity()) {
        if (__n) this->_S_copy(this->_M_data() + this->size(), __s, __n);  // append 到当前数据的尾部
    } else
        this->_M_mutate(this->size(), size_type(0), __s, __n); //

    this->_M_set_length(__len);
    return *this;
}

// When __n = 1 way faster than the general multichar
// traits_type::copy/move/assign.
static void _S_copy(_CharT* __d, const _CharT* __s, size_type __n) {
    if (__n == 1)
        traits_type::assign(*__d, *__s);
    else
        // 把 _s 的 _n 个字符copy 到 _d 中, _d 是首地址
        // 同 std::copy， 即 std::copy(__s, __s + __n, __d);
        traits_type::copy(__d, __s, __n);
}
template <typename _CharT, typename _Traits, typename _Alloc>
typename basic_string<_CharT, _Traits, _Alloc>::pointer basic_string<_CharT, _Traits, _Alloc>::
_M_create(size_type& __capacity, size_type __old_capacity) {
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 83.  String::npos vs. string::max_size()
    if (__capacity > max_size()) std::__throw_length_error(__N("basic_string::_M_create"));

    // The below implements an exponential growth policy, necessary to
    // meet amortized linear time requirements of the library: see
    // http://gcc.gnu.org/ml/libstdc++/2001-07/msg00085.html.
    if (__capacity > __old_capacity && __capacity < 2 * __old_capacity) {
        // 如果不大于原来的 2 倍，则分配 2 倍的空间，否则按实际分配.
        __capacity = 2 * __old_capacity;
        // Never allocate a string bigger than max_size.
        if (__capacity > max_size()) __capacity = max_size();
    }

    // NB: Need an array of char_type[__capacity], plus a terminating
    // null char_type() element.
    // 即实际分配时，还是要多添加一个字符 terminating null char.
    return _Alloc_traits::allocate(_M_get_allocator(), __capacity + 1);
}

template <typename _CharT, typename _Traits, typename _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_mutate(size_type __pos, size_type __len1, const _CharT* __s,
                                                      size_type __len2) {
    const size_type __how_much = length() - __pos - __len1;

    size_type __new_capacity = length() + __len2 - __len1;
    pointer __r = _M_create(__new_capacity, capacity());

    if (__pos) this->_S_copy(__r, _M_data(), __pos);
    if (__s && __len2) this->_S_copy(__r + __pos, __s, __len2);
    if (__how_much) this->_S_copy(__r + __pos + __len2, _M_data() + __pos + __len1, __how_much);

    _M_dispose();
    _M_data(__r);
    _M_capacity(__new_capacity);
}

Summary:
- 当添加新的字符时，如果 new size > capacity, 才会触发空间重新分配
  - 再重新分配空间时，当 new capacity < 2* old capacity 时，则分配 2* old capacity, 否则以实际分配.
    - 这里的 new capacity 是 字符的实际数量，即 length() + appending_string.length().
*/

TEST(string, within_default_capacity_without_reallocation) {
    std::string str;
    EXPECT_EQ(0, str.length());
    EXPECT_EQ(15, str.capacity());

    str = generate_random_alphanumeric_string(15);
    EXPECT_EQ(15, str.length());
    EXPECT_EQ(15, str.capacity());

    /*
    15 是因为
    enum { _S_local_capacity = 15 / sizeof(_CharT) };
    */
}

TEST(string, runs_out_of_room_and_reallocate_double_capacity) {
    std::string str;
    EXPECT_EQ(0, str.length());
    EXPECT_EQ(15, str.capacity());

    str += generate_random_alphanumeric_string(16);
    EXPECT_EQ(16, str.length());
    EXPECT_EQ(30, str.capacity());
}

TEST(string, runs_out_of_room_and_reallocate_the_actual_capacity) {
    std::string str;
    EXPECT_EQ(0, str.length());
    EXPECT_EQ(15, str.capacity());

    // 但新需求的空间greater than 原来的两倍时(not in (old_capacity, 2*old_capacity))，按实际的分配
    str += generate_random_alphanumeric_string(31);
    EXPECT_EQ(31, str.length());
    EXPECT_EQ(31, str.capacity());
}