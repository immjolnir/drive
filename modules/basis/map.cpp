#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <map>
#include <unordered_map>
/*
- map
  - map 内部实现了一个红黑树，该结构具有自动排序的功能，
  - 因此 map 内部的所有元素都是有序的，红黑树的每一个节点都代表着 map 的一个元素，
  - 因此，对于 map 进行的查找，删除，添加等一系列的操作都相当于是对红黑树进行这样的操作，故红黑树的效率决定了map的效率.
  - 查询复杂度O(logn)

- unorded_map
  - unordered_map内部实现了一个哈希表，因此其元素的排列顺序是杂乱的，无序的
  - 查询复杂度O(1)
  
占用内存方面：map内存占用略低，unordered_map内存占用略高,而且是线性成比例的。


*/

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

TEST(std_map, map_traversal_iterator) {
    std::map<int, int> m = {{1, 1}, {2, 2}, {3, 3}};
    std::vector<int> vec;
    for (auto it = m.begin(); it != m.end(); ++it) {
        vec.emplace_back(it->first);
    }
    EXPECT_THAT(vec, testing::ElementsAre(1, 2, 3));
}

TEST(std_map, map_traversal_range_for_since_std11) {
    std::map<int, int> m = {{1, 1}, {2, 2}, {3, 3}};
    std::vector<int> vec;
    for (auto item : m) {  // since c++11
        vec.emplace_back(item.first);
    }
    EXPECT_THAT(vec, testing::ElementsAre(1, 2, 3));
}

TEST(std_map, map_traversal_since_std17) {
    std::map<int, int> m = {{1, 1}, {2, 2}, {3, 3}};
    std::vector<int> vec;
    for (auto [key, value] : m) {
        vec.emplace_back(key);
    }
    EXPECT_THAT(vec, testing::ElementsAre(1, 2, 3));
}