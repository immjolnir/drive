# Forward Declaration

The purpose is for Optimizing C++ header files.

C++ is a dynamic language, which requires a proactive approach to try and reduce compile-time dependencies. A common way of doing so involves moving the dependencies from the headers to source files. This is typically achieved by using a functionality called Forward Declaration.

In a nutshell, the declaration just tells the compiler what parameters the function takes and returns, while the actual definition takes care of the actual work.
Let’s look at two guidelines that should improve your compilation times and decrease portability issues, as well as include-what-you-use, a tool that automatically applies these guidelines in your codebase.

- Forward declare types to speed up compile time

The compiler also doesn’t need the definition of `std::string` and `std::ostream` but because they are templates (with potentially additional default templated parameters we don’t know), we can’t forward declare them.

Luckily, for `std::ostream` the header `<iosfwd>` provides a forward declarations.

Even if the definition of `std::ostream` was needed, `<ostream>` provides only it without the input streams or `std::cin`, `std::cout`, …

- Include portability issues
  - [include what you use](https://github.com/include-what-you-use/include-what-you-use)

After building it from source or getting one of the pre-built binaries from here, running it is relatively easy if your using make or CMake. With make all you need to do is set the CXX variable to IWYU, with CMake just set the CMAKE_CXX_INCLUDE_WHAT_YOU_USE option. Check their documentation for more details. You can also run it manually, it accepts the same options as clang itself.

The tool will give you a summary of changes you should make to each file. For our example file above it prints:

header.hpp should add these lines:
#include <string> // for string
class B;

header.hpp should remove these lines:
– #include “b.hpp” // lines 6-6

The full include-list for header.hpp:
#include <iostream> // for ostream
#include <string> // for string
#include “a.hpp” // for A
class B;

There is also python script fix_includes.py. If you give it the output of IWYU, it will automatically apply those changes.

Guideline: Occasionally run IWYU over your codebase. It can improve compilation times and improve portability.



