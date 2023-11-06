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
