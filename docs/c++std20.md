# CC20

## range-based `for` loops
```c++
template <typename T>
std::ostream& operator<<(std::ostream& s, const std::forward_list<T>& v) {
    s.put('{');

    // warning: range-based 'for' loops with initializer only available with '-std=c++20' or '-std=gnu++20'
    for (char comma[]{'\0', ' ', '\0'}; const auto& e : v) {  // What's this?
        s << comma << e, comma[0] = ',';
    }
    return s << "}\n";
}
```
- modules/basis/forward_list.cpp

#include <ranges>

static constexpr std::map<int, int> EDGES = {{0, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 7}, {4, 8}, {5, 8}};
    // https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-
    // a-vector
    auto ks = std::views::keys(EDGES);  // since c++20
    auto begin = ks.begin();
    for (; begin != ks.end(); ++begin) {
        auto from = *begin;
        auto to = EDGES[from];
        uf.union_them(from, to);
    }