#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

/*
https://en.cppreference.com/w/cpp/algorithm/iter_swap

template< class ForwardIt1, class ForwardIt2>
void iter_swap( ForwardIt1 a, ForwardIt2 b );

- Swaps the values of the elements the given iterators are pointing to.

- Possible implementation

template<class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) // constexpr since C++20
{
    using std::swap;
    swap(*a, *b);
}
*/
#if __cplusplus > 201703L
using StringView = std::string_view;
#else
using StringView = const std::string&;
#endif

template <class ForwardIt>
void selection_sort(ForwardIt begin, ForwardIt end) {
    for (ForwardIt it = begin; it != end; ++it) {
        std::iter_swap(it, std::min_element(it, end));
    }
}

void println(StringView rem, std::vector<int> const& v) {
    std::cout << rem;
    for (int e : v) std::cout << e << ' ';
    std::cout << '\n';
}

template <int min, int max>
int rand_int() {
    static std::uniform_int_distribution dist(min, max);
    static std::mt19937 gen(std::random_device{}());
    return dist(gen);
}

int main() {
    std::vector<int> v;
    std::generate_n(std::back_inserter(v), 20, rand_int<-9, +9>);

    std::cout << std::showpos;
    println("Before sort: ", v);
    selection_sort(v.begin(), v.end());
    println("After sort:  ", v);
}