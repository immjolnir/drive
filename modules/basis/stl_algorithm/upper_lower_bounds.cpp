#include <gtest/gtest.h>

#include <algorithm>  // for lower_bound, upper_bound and sort
#include <map>
#include <vector>

/*
A fully-sorted range meets this criterion.

https://www.mathsisfun.com/definitions/upper-bound.html

- In mathmatics,
  - Upper Bound
  A value that is greater than or equal to every element of a set of data.

  Example: in {3,5,11,20,22} 22 is an upper bound

  But be careful! 23 is also an upper bound (it is greater than any element of that set), in fact any value 22 or above
is an upper bound, such as 50 or 1000.

  Example: how many hours of sunlight today? We may not know exactly when sunrise and sunset are, or if it will be
cloudy or not, but we can say for sure that 24 Hours is an upper bound.

- In c++

std::lower_bound - returns iterator to first element in the given range which is EQUAL_TO or Greater than val.

std::upper_bound - returns iterator to first element in the given range which is Greater than val.

  - https://stackoverflow.com/questions/41958581/difference-between-upper-bound-and-lower-bound-in-stl

    value a a a b b b c c c
    index 0 1 2 3 4 5 6 7 8
    bound       l     u

    Where l represents the lower bound of b, and u represents the upper bound of b.

    So if there are range of values that are "equal" with respect to the comparison being used,
    lower_bound gives you the first of this,
    upper_bound gives you one-past-the-end of these.
    This is the normal pattern of STL ranges [first, last).
*/

TEST(upper_lower_bounds, find_the_range) {
    std::vector<char> vec = {'a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c'};

    std::vector<char>::iterator itlow, itup;
    itlow = std::lower_bound(vec.begin(), vec.end(), 'b');
    itup = std::upper_bound(vec.begin(), vec.end(), 'b');

    EXPECT_TRUE(itlow != vec.end());
    EXPECT_EQ(3, std::distance(vec.begin(), itlow));
    EXPECT_EQ('b', *itlow);

    EXPECT_TRUE(itup != vec.end());
    EXPECT_EQ(6, std::distance(vec.begin(), itup));
    EXPECT_EQ('c', *itup);
}

TEST(upper_lower_bounds, erase_a_range) {
    // https://stackoverflow.com/questions/30209168/how-to-find-all-elements-in-a-range-in-stl-mapset
    std::map<char, int> mymap;
    std::map<char, int>::iterator itlow, itup;

    mymap['a'] = 20;
    mymap['b'] = 40;
    mymap['c'] = 60;
    mymap['d'] = 80;
    mymap['e'] = 100;

    /*
                               itup
                                v
    Find the range [a, b, c, d, e]
                       ^
                     itlow
    */
    itlow = mymap.lower_bound('b');  // itlow points to b
    itup = mymap.upper_bound('d');   // itup points to e (not d!)

    EXPECT_TRUE(itlow != mymap.end());
    EXPECT_EQ('b', itlow->first);

    EXPECT_TRUE(itup != mymap.end());
    EXPECT_EQ('e', itup->first);

    mymap.erase(itlow, itup);  // erases [itlow,itup)

    // print content:
    for (std::map<char, int>::iterator it = mymap.begin(); it != mymap.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
}
