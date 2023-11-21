#include <gtest/gtest.h>

#include <iostream>

/*
https://google.github.io/googletest/advanced.html#sharing-resources-between-tests-in-the-same-test-suite
*/

struct Foo {
    Foo() { std::cout << "Foo's ctor " << std::endl; }

    ~Foo() { std::cout << "Foo's dtor " << std::endl; }
};

class FooTest : public testing::Test {
  protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() {
        shared_resource_ = new Foo;

        // If `shared_resource_` is **not deleted** in `TearDownTestSuite()`,
        // reallocation should be prevented because `SetUpTestSuite()` may be called
        // in subclasses of FooTest and lead to memory leak.
        //
        // if (shared_resource_ == nullptr) {
        //   shared_resource_ = new ...;
        // }
        std::cout << "SetUpTestSuite()" << std::endl;
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() {
        delete shared_resource_;
        shared_resource_ = nullptr;

        std::cout << "TearDownTestSuite()" << std::endl;
    }

    // You can define per-test set-up logic as usual.
    void SetUp() override { std::cout << "SetUp()" << std::endl; }

    // You can define per-test tear-down logic as usual.
    void TearDown() override { std::cout << "TearDown()" << std::endl; }

    // Some expensive resource shared by all tests.
    static Foo* shared_resource_;  // Just declare it
};

// error: undefined reference to 'FooTest::shared_resource_' when missing it
Foo* FooTest::shared_resource_ = nullptr;  // Definition

TEST_F(FooTest, Test1) { std::cout << "case Test1" << std::endl; }

TEST_F(FooTest, Test2) { std::cout << "case Test2" << std::endl; }

/*
$ ./build/modules/google_gtest/test_suite
Running main() from ./googletest/src/gtest_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from FooTest
Foo's ctor
SetUpTestSuite()
[ RUN      ] FooTest.Test1
SetUp()
case Test1
TearDown()
[       OK ] FooTest.Test1 (0 ms)
[ RUN      ] FooTest.Test2
SetUp()
case Test2
TearDown()
[       OK ] FooTest.Test2 (0 ms)
Foo's dtor
TearDownTestSuite()
[----------] 2 tests from FooTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
*/
