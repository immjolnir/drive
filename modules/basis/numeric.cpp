#include <gtest/gtest.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>

/* iota
https://en.cppreference.com/w/cpp/algorithm/iota

template< class ForwardIt, class T >
void iota( ForwardIt first, ForwardIt last, T value );

Fills the range [first, last) with sequentially increasing values, starting with value and repetitively evaluating
++value. Equivalent operation:

*(first)     = value;
*(first + 1) = ++value;
*(first + 2) = ++value;
*(first + 3) = ++value;
...
*/

class BigData  // inefficient to copy
{
    int data[1024]; /* some raw data */
  public:
    explicit BigData(int i = 0) { data[0] = i; /* ... */ }

    operator int() const { return data[0]; }

    BigData& operator=(int i) {
        data[0] = i;
        return *this;
    }

    /* ... */
};

TEST(numeric, iota) {
    std::list<BigData> l(10);
    std::iota(l.begin(), l.end(), -4);

    std::vector<std::list<BigData>::iterator> v(l.size());
    std::iota(v.begin(), v.end(), l.begin());
    // Vector of iterators (to original data) is used to avoid expensive copying,
    // and because std::shuffle (below) cannot be applied to a std::list directly.

    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

    std::cout << "Original contents of the list l:\t";
    for (auto const& n : l) std::cout << std::setw(2) << n << ' ';
    std::cout << '\n';

    std::cout << "Contents of l, viewed via shuffled v:\t";
    for (auto const i : v) std::cout << std::setw(2) << *i << ' ';
    std::cout << '\n';

    EXPECT_NE(v[0], l.begin());
}