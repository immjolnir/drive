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