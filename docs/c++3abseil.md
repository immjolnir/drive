# [absel](https://abseil.io/): Compatibility with the C++ Standard
Google has developed many abstractions that either match or closely match features incorporated into C++14, C++17, and beyond.
Using the Abseil versions of these abstractions allows you to access these features now, even if your code is not yet ready for life in a post C++11 world.


## [absl::StrFormat()](https://abseil.io/docs/cpp/guides/format)
The `str_format` library is a typesafe replacement for the family of `printf()` string formatting routines within the `<cstdio>` standard library header.
The `str_format` library provides most of the functionality of `printf()` type string formatting and a number of additional benefits:

- Type safety, including native support for `std::string `and `absl::string_view`

```c++
#include "absl/strings/str_format.h"

std::string s = absl::StrFormat("Welcome to %s, Number %d!", "The Village", 6);
EXPECT_EQ("Welcome to The Village, Number 6!", s);
```