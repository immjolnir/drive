#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <forward_list>
#include <iostream>
#include <string>

template <typename T>
std::ostream& operator<<(std::ostream& s, const std::forward_list<T>& v) {
    s.put('{');

    // warning: range-based 'for' loops with initializer only available with '-std=c++20' or '-std=gnu++20'
    for (char comma[]{'\0', ' ', '\0'}; const auto& e : v) {  // What's this?
        s << comma << e, comma[0] = ',';
    }
    return s << "}\n";
}

TEST(std_forward_list, ctor) {
    // C++11 initializer list syntax:
    std::forward_list<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    EXPECT_THAT(words1, testing::ElementsAre("the", "frogurt", "is", "also", "cursed"));

    // words2 == words1
    std::forward_list<std::string> words2(words1.begin(), words1.end());
    EXPECT_THAT(words2, testing::ElementsAre("the", "frogurt", "is", "also", "cursed"));

    // words3 == words1
    std::forward_list<std::string> words3(words1);
    EXPECT_THAT(words3, testing::ElementsAre("the", "frogurt", "is", "also", "cursed"));

    std::forward_list<std::string> words4(5, "Mo");
    EXPECT_THAT(words4, testing::ElementsAre("Mo", "Mo", "Mo", "Mo", "Mo"));

    auto const rg = {"cat", "cow", "crow"};
#ifdef __cpp_lib_containers_ranges
    /*
    template< container-compatible-range<T> R >
    forward_list( std::from_range_t, R&& rg, const Allocator& alloc = Allocator() ); (since C++23)
    */
    std::forward_list<std::string> words5(std::from_range, rg);  // overload (11)
#else
    /*
    template< class InputIt >
    forward_list( InputIt first, InputIt last, const Allocator& alloc = Allocator() ); (since c++11)
    */
    std::forward_list<std::string> words5(rg.begin(), rg.end());  // overload (5)
#endif
    EXPECT_THAT(words5, testing::ElementsAre("cat", "cow", "crow"));
}