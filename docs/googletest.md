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
