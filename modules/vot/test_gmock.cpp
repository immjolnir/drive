// A Guide To Implementing And Testing In C++ And Gmock
// https://www.amplifyre.com/articles/guide-implementing-and-testing-in-c-and-gmock

/*
Callbacks exist in real life: you call someone, and they can not answer at that moment for any reason, you request a
callback so that they call you at a later time. C++ has some Callables: functions, lambda expressions, bind expressions,
function objects, and function pointers.

There is also a class template std::function which is a general-purpose function wrapper that can store, copy and invoke
any Callable.
*/

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <functional>
#include <memory>
#include <thread>  // this_thread::sleep_for

using ::testing::_;
using ::testing::AtMost;
using ::testing::DoDefault;
using ::testing::MockFunction;
using ::testing::Return;

class User {
  public:
    User(std::function<void()> futureTask) : m_futureTask(futureTask) {}

    void userAction() { m_futureTask(); }

  private:
    std::function<void()> m_futureTask;
};

class UserTest : public ::testing::Test {
  public:
    UserTest() { m_user.reset(new User(m_mockFutureTask.AsStdFunction())); }

  protected:
    std::unique_ptr<User> m_user;
    MockFunction<std::function<void()>> m_mockFutureTask;
};

TEST_F(UserTest, testfutureTask) {
    EXPECT_CALL(m_mockFutureTask, Call());
    m_user->userAction();
}

// https://levelup.gitconnected.com/mocking-with-googletest-gtest-6dde5230e7aa
class Sandman {
  public:
    virtual void foo(int x) {
        // For a rough first look, we assume this function is “problematic”: in it, we sleep for 10s before returning,
        // which we do not want to do in a unit test → we want to mock it.
        std::cout << "foo will sleep for " << x << " second." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(x));
    }

    virtual int bar() { return 10; }

    virtual int add(int x, int y) { return x + y; }

    virtual ~Sandman() = default;
};

// TEST(SandmanTestCase, TestFoo) {
//     Sandman example;
//     example.foo(2);
// }

class SandmanMocked : public Sandman {
  public:
    MOCK_METHOD(void, foo, (int), (override));

    // Return values from the mocked function
    MOCK_METHOD(int, bar, (), (override));

    // If we want the example to be a bit fancier / more realistic, let’s change the code s.t. foo takes a parameter as
    // input and returns that — correspondingly we modify the expected return in the unit test:
    MOCK_METHOD(int, add, (int, int), (override));
};

TEST(SandmanTest, TestMockFoo) {
    SandmanMocked example = SandmanMocked();
    EXPECT_CALL(example, foo(2)).Times(1);
    example.foo(2);
}

TEST(SandmanTest, TestMockBar) {
    SandmanMocked example = SandmanMocked();
    EXPECT_CALL(example, bar()).Times(1).WillOnce(::testing::Return(10));
    EXPECT_EQ(example.bar(), 10);
}

TEST(SandmanTest, TestMockAdd) {
    SandmanMocked example = SandmanMocked();
    EXPECT_CALL(example, add(1, 1)).Times(1).WillOnce(::testing::Return(2));
    EXPECT_EQ(example.add(1, 1), 2);

    // 1 + 2 = 2, why?
    EXPECT_CALL(example, add(1, 2)).Times(1).WillOnce(::testing::Return(2));
    EXPECT_EQ(example.add(1, 2), 2);
}

// https://github.com/google/googletest/blob/main/googlemock/test/gmock_stress_test.cc
class MockFoo {
  public:
    MOCK_METHOD1(Bar, int(int n));                                   // NOLINT
    MOCK_METHOD2(Baz, char(const char* s1, const std::string& s2));  // NOLINT
};

TEST(MockFooTest, overrideMethod) {
    // Creates a mock and does some typical operations on it.
    MockFoo foo;
    ON_CALL(foo, Bar(_)).WillByDefault(Return(1));
    ON_CALL(foo, Baz(_, _)).WillByDefault(Return('b'));
    ON_CALL(foo, Baz(_, "you")).WillByDefault(Return('a'));

    EXPECT_CALL(foo, Bar(0)).Times(AtMost(3));
    EXPECT_CALL(foo, Baz(_, _));
    EXPECT_CALL(foo, Baz("hi", "you")).WillOnce(Return('z')).WillRepeatedly(DoDefault());

    EXPECT_EQ(1, foo.Bar(0));
    EXPECT_EQ(1, foo.Bar(0));
    EXPECT_EQ('z', foo.Baz("hi", "you"));
    EXPECT_EQ('a', foo.Baz("hi", "you"));
    EXPECT_EQ('b', foo.Baz("hi", "me"));
}

// Because the gtest_main is linked.
// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
