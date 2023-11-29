#include <gtest/gtest.h>

#include <map>
#include <unordered_map>

/** How to get the value or default in c++'s map?
 *
 * https://stackoverflow.com/questions/2333728/stdmap-default-value

No, there isn't. The simplest solution is to write your own free template function to do this. Something like:

Like this: get(key[, default])
*/

#if __cplusplus >= 201103L
// Purpose: Account for generic associative containers, as well as optional comparator and allocator parameters.
template <template <class, class, class...> class C, typename K, typename V, typename... Args>
V GetWithDef(const C<K, V, Args...>& m, K const& key, const V& defval) {
    typename C<K, V, Args...>::const_iterator it = m.find(key);
    if (it == m.end()) return defval;
    return it->second;
}

// It's ok with:
/*
template <typename C, typename K, typename V, typename... Args>
V GetWithDef(const C& m, K const& key, const V& defval) {
    typename C::const_iterator it = m.find(key);
    if (it == m.end()) return defval;
    return it->second;
}
*/
// But which is better and why?

#else
template <typename K, typename V>
V GetWithDef(const std::map<K, V>& m, const K& key, const V& defval) {
    typename std::map<K, V>::const_iterator it = m.find(key);
    if (it == m.end()) {
        return defval;
    } else {
        return it->second;
    }
}
#endif

#if __cplusplus > 201703L
// C++17 provides try_emplace which does exactly this.
// It takes a key and an argument list for the value constructor and returns a pair: an iterator and a bool.
// http://en.cppreference.com/w/cpp/container/map/try_emplace
#endif

TEST(std_map, get_or_default) {
    std::map<int, int> m = {{1, 1}, {2, 2}};
    EXPECT_EQ(1, GetWithDef(m, 1, -1));
    EXPECT_EQ(2, GetWithDef(m, 2, -1));
    EXPECT_EQ(-1, GetWithDef(m, 3, -1));  // Get the default;
}