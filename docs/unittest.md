# Unit Test

## [assertions](http://google.github.io/googletest/primer.html#assertions)
  
GoogleTest assertions are macros that resemble function calls. You test a class or function by making assertions about its behavior. When an assertion fails, GoogleTest prints the assertion’s source file and line number location, along with a failure message. You may also supply a custom failure message which will be appended to GoogleTest’s message.

The assertions come in pairs that test the same thing but have different effects on the current function. 
> `ASSERT_*` versions generate fatal failures when they fail, and abort the current function. 
> `EXPECT_*` versions generate nonfatal failures, which don’t abort the current function. 

Usually `EXPECT_*` are preferred, as they allow more than one failure to be reported in a test. However, you should use `ASSERT_*` if it doesn’t make sense to continue when the assertion in question fails.

Since a failed `ASSERT_*` returns from the current function immediately, possibly skipping clean-up code that comes after it, it may cause a space leak. Depending on the nature of the leak, it may or may not be worth fixing - so keep this in mind if you get a heap checker error in addition to assertion errors.

To provide a custom failure message, simply stream it into the macro using the `<< operator` or a sequence of such operators. See the following example, using the `ASSERT_EQ` and `EXPECT_EQ` macros to verify value equality:

```c++
ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

for (int i = 0; i < x.size(); ++i) {
  EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
}
```

Anything that can be streamed to an `ostream` can be streamed to an assertion macro–in particular, C strings and string objects. If a wide string (wchar_t*, TCHAR* in UNICODE mode on Windows, or std::wstring) is streamed to an assertion, it will be translated to UTF-8 when printed.

GoogleTest provides a collection of assertions for verifying the behavior of your code in various ways. You can check Boolean conditions, compare values based on relational operators, verify string values, floating-point values, and much more.

- [EXPECT_ vs ASSERT_ in gtest ](https://stackoverflow.com/questions/2565299/using-assert-and-expect-in-googletest)
