# gtest

- [How to skip a test in gtest](https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test)

```c++
GTEST_SKIP() << "Skipping the single test";
```

- [How to test a program with crash?](https://stackoverflow.com/questions/6569713/testing-for-crash-with-google-test)

You can use a death test to isolate a crash:

```c++
EXPECT_EXIT(ImNotNiceToPointers(p); exit(0), ExitedWithCode(0), '');
```

However, I recommend against using death tests. Using a death test incurs the overhead of launching a subprocess whether
there is a crash or not. But if you simply leave your code as is and your test crashes, you will know, and will be able
to fix it.

Tracing the origin of the crash is easy with the help of tools like Valgrind or Dr. Memory.

- Comparing array/list
```c++
#include <gmock/gmock.h>

    EXPECT_THAT(vec, testing::ElementsAre(1, 2, 3, 4, 5));
```

- Handle Exception Test Case

Using `FAIL() << ..` to test it.

```c++
    // modules/basis/exception_stardardarized_example.cpp
    try {
        // Code that could throw an exception
        md = GetNetworkResource();
    } catch (const networkIOException& e) {
        FAIL() << "Expected ellipsis Exception 1";
    } catch (const myDataFormatException& e) {
        FAIL() << "Expected ellipsis Exception 2";
    } catch (const std::exception& e) {
        FAIL() << "Expected ellipsis Exception 3";
    } catch (...) {
        // Here we cannot get it
        // https://stackoverflow.com/questions/14232814/how-do-i-make-a-call-to-what-on-stdexception-ptr
        // An opaque pointer to an arbitrary exception
        std::cerr << "ellipsis handler deeper] who knows!" << std::endl;
        eptr = std::current_exception();  // capture
        EXPECT_EQ(15, age);
    }
```
- /usr/include/gtest/gtest_prod.h

```c++
// When you need to test the private or protected members of a class,
// use the FRIEND_TEST macro to declare your tests as friends of the
// class.  For example:
//
// class MyClass {
//  private:
//   void MyMethod();
//   FRIEND_TEST(MyClassTest, MyMethod);
// };
//
// class MyClassTest : public testing::Test {
//   // ...
// };
//
// TEST_F(MyClassTest, MyMethod) {
//   // Can call MyClass::MyMethod() here.
// }

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test
```

## Skip test
- https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test
- https://google.github.io/googletest/advanced.html

```c++
TEST(SkipTest, DoesSkip) {
  GTEST_SKIP() << "Skipping single test";
  EXPECT_EQ(0, 1);  // Won't fail; it won't be executed
}

class SkipFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    GTEST_SKIP() << "Skipping all tests for this fixture";
  }
};

// Tests for SkipFixture won't be executed.
TEST_F(SkipFixture, SkipsOneTest) {
  EXPECT_EQ(5, 7);  // Won't fail
}
```

## How to catch segmentation fault with Google Test?
- /usr/include/gtest/gtest-death-test.h
```
// Here's what happens when an ASSERT_DEATH* or EXPECT_DEATH* is
// executed:
//
//   1. It generates a warning if there is more than one active
//   thread.  This is because it's safe to fork() or clone() only
//   when there is a single thread.
//
//   2. The parent process clone()s a sub-process and runs the death
//   test in it; the sub-process exits with code 0 at the end of the
//   death test, if it hasn't exited already.
//
//   3. The parent process waits for the sub-process to terminate.
//
//   4. The parent process checks the exit code and error message of
//   the sub-process.
//
// Examples:
//
//   ASSERT_DEATH(server.SendMessage(56, "Hello"), "Invalid port number");
//   for (int i = 0; i < 5; i++) {
//     EXPECT_DEATH(server.ProcessRequest(i),
//                  "Invalid request .* in ProcessRequest()")
//                  << "Failed to die on request " << i;
//   }
//
//   ASSERT_EXIT(server.ExitNow(), ::testing::ExitedWithCode(0), "Exiting");
//
//   bool KilledBySIGHUP(int exit_code) {
//     return WIFSIGNALED(exit_code) && WTERMSIG(exit_code) == SIGHUP;
//   }
//
//   ASSERT_EXIT(client.HangUpServer(), KilledBySIGHUP, "Hanging up!");
//
// The final parameter to each of these macros is a matcher applied to any data
// the sub-process wrote to stderr.  For compatibility with existing tests, a
// bare string is interpreted as a regular expression matcher.
//
// On the regular expressions used in death tests:
//
//   GOOGLETEST_CM0005 DO NOT DELETE
//   On POSIX-compliant systems (*nix), we use the <regex.h> library,
//   which uses the POSIX extended regex syntax.
//
//   On other platforms (e.g. Windows or Mac), we only support a simple regex
//   syntax implemented as part of Google Test.  This limited
//   implementation should be enough most of the time when writing
//   death tests; though it lacks many features you can find in PCRE
//   or POSIX extended regex syntax.  For example, we don't support
//   union ("x|y"), grouping ("(xy)"), brackets ("[xy]"), and
//   repetition count ("x{5,7}"), among others.
//
//   Below is the syntax that we do support.  We chose it to be a
//   subset of both PCRE and POSIX extended regex, so it's easy to
//   learn wherever you come from.  In the following: 'A' denotes a
//   literal character, period (.), or a single \\ escape sequence;
//   'x' and 'y' denote regular expressions; 'm' and 'n' are for
//   natural numbers.
//
//     c     matches any literal character c
//     \\d   matches any decimal digit
//     \\D   matches any character that's not a decimal digit
//     \\f   matches \f
//     \\n   matches \n
//     \\r   matches \r
//     \\s   matches any ASCII whitespace, including \n
//     \\S   matches any character that's not a whitespace
//     \\t   matches \t
//     \\v   matches \v
//     \\w   matches any letter, _, or decimal digit
//     \\W   matches any character that \\w doesn't match
//     \\c   matches any literal character c, which must be a punctuation
//     .     matches any single character except \n
//     A?    matches 0 or 1 occurrences of A
//     A*    matches 0 or many occurrences of A
//     A+    matches 1 or many occurrences of A
//     ^     matches the beginning of a string (not that of each line)
//     $     matches the end of a string (not that of each line)
//     xy    matches x followed by y
//
//   If you accidentally use PCRE or POSIX extended regex features
//   not implemented by us, you will get a run-time failure.  In that
//   case, please try to rewrite your regular expression within the
//   above syntax.
//
//   This implementation is *not* meant to be as highly tuned or robust
//   as a compiled regex library, but should perform well enough for a
//   death test, which already incurs significant overhead by launching
//   a child process.
//
// Known caveats:
//
//   A "threadsafe" style death test obtains the path to the test
//   program from argv[0] and re-executes it in the sub-process.  For
//   simplicity, the current implementation doesn't search the PATH
//   when launching the sub-process.  This means that the user must
//   invoke the test program via a path that contains at least one
//   path separator (e.g. path/to/foo_test and
//   /absolute/path/to/bar_test are fine, but foo_test is not).  This
//   is rarely a problem as people usually don't put the test binary
//   directory in PATH.
//

// Asserts that a given `statement` causes the program to exit, with an
// integer exit status that satisfies `predicate`, and emitting error output
// that matches `matcher`.
# define ASSERT_EXIT(statement, predicate, matcher) \
    GTEST_DEATH_TEST_(statement, predicate, matcher, GTEST_FATAL_FAILURE_)

// Like `ASSERT_EXIT`, but continues on to successive tests in the
// test suite, if any:
# define EXPECT_EXIT(statement, predicate, matcher) \
    GTEST_DEATH_TEST_(statement, predicate, matcher, GTEST_NONFATAL_FAILURE_)

// Asserts that a given `statement` causes the program to exit, either by
// explicitly exiting with a nonzero exit code or being killed by a
// signal, and emitting error output that matches `matcher`.
# define ASSERT_DEATH(statement, matcher) \
    ASSERT_EXIT(statement, ::testing::internal::ExitedUnsuccessfully, matcher)

// Like `ASSERT_DEATH`, but continues on to successive tests in the
// test suite, if any:
# define EXPECT_DEATH(statement, matcher) \
    EXPECT_EXIT(statement, ::testing::internal::ExitedUnsuccessfully, matcher)
```
- ASSERT_EXIT
- EXPECT_EXIT
- ASSERT_DEATH
- EXPECT_DEATH

- Example: modules/google_gtest/test_death.cpp

## GTEST_BREAK_ON_FAILURE 是怎么工作的?
GTEST_BREAK_ON_FAILURE 是 Google Test（gtest）框架中的一个断言宏。
当在测试代码中使用这个宏时，如果相应的断言条件失败，Google Test 将会调用平台相关的函数来暂停程序执行，通常这会导致调试器在这个点上设置一个断点。

要让 GTEST_BREAK_ON_FAILURE 起作用，你需要确保你的测试环境中启用了特定的标志或选项。
例如，在命令行运行测试时，可以使用 `–gtest_break_on_failure `参数。
这样，当任何断言失败时，程序会在该位置停下来，允许你通过调试器检查当时的变量值、调用栈等信息，以便于定位和解决问题。

下面是使用 GTEST_BREAK_ON_FAILURE 的例子：
```c++
#include <gtest/gtest.h>

TEST(MyTest, FailingCase) { 
    int a = 0; GTEST_ASSERT_EQ(a, 1); 
    // This will fail and halt execution. 
}
```
当你运行包含此测试用例的测试程序，并添加了 `–gtest_break_on_failure` 参数时，
当执行到 `GTEST_ASSERT_EQ(a, 1)` 这一行并且条件不满足时，程序就会立即暂停。
在调试器环境下，你可以查看变量的当前值并逐步执行以了解问题所在。

记住，GTEST_BREAK_ON_FAILURE 只是一个辅助调试的工具，并不会改变测试的结果。即使没有启用这个选项，测试仍然会报告为失败，只是程序不会被中断。

