#include <gtest/gtest.h>

#include <boost/bind/bind.hpp>

int func() { return 101; }

class Student {
  public:
    bool gt101(int i) { return i > 101; }

    int add(int i, int j, int k) { return i - j + k; }
};

TEST(boost, bind) {
    auto foo = boost::bind(&func);
    EXPECT_EQ(101, foo());

    Student st;
    auto bar = boost::bind(&Student::gt101, &st, 5);
    EXPECT_FALSE(bar());

    // when includes
    // #include <boost/bind.hpp>
    //  note: '#pragma message: The practice of declaring the Bind placeholders (_1, _2, ...) in the global namespace is
    //  deprecated. Please use <boost/bind/bind.hpp> + using namespace boost::placeholders, or define
    //  BOOST_BIND_GLOBAL_PLACEHOLDERS to retain the current behavior.'
    //    36 | BOOST_PRAGMA_MESSAGE(
    auto foz = boost::bind(&Student::add, &st, boost::placeholders::_1, 10, boost::placeholders::_2);
    EXPECT_EQ(-7, foz(1, 2));
    EXPECT_EQ(0, foz(5, 5));
}