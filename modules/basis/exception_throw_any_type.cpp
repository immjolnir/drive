#include <stdexcept>

/*
* https://www.geeksforgeeks.org/exception-handling-c/

- Exception handling in C++ consist of three keywords: `try`, `throw` and `catch`:
  - The `try` statement allows you to define a block of code to be tested for errors while it is being executed.

  - The `throw` keyword throws an exception when a problem is detected, which lets us create a custom error.

  - The `catch` statement allows you to define a block of code to be executed, if an error occurs in the try block.

* https://learn.microsoft.com/en-us/cpp/cpp/try-throw-and-catch-statements-cpp?view=msvc-170
To implement exception handling in C++, you use try, throw, and catch expressions.

First, use a try block to enclose one or more statements that might throw an exception.

A throw expression signals that an exceptional condition—often, an error—has occurred in a try block. You can use an
object of any type as the operand of a throw expression. Typically, this object is used to communicate information about
the error. In most cases, we recommend that you use the std::exception class or one of the derived classes that are
defined in the standard library. If one of those isn't appropriate, we recommend that you derive your own exception
class from std::exception.

To handle exceptions that may be thrown, implement one or more catch blocks immediately following a try block. Each
catch block specifies the type of exception it can handle.

*/

#include <exception>
#include <iostream>

#include <gtest/gtest.h>

TEST(exception, throw_an_type) {
    try {
        int age = 15;
        if (age >= 18) {
            std::cout << "Access granted - you are old enough." << std::endl;
        } else {
            throw(age);
        }
    } catch (int myNum) {
        std::cout << "Access denied - You must be at least 18 years old.\n";
        std::cout << "Age is: " << myNum << std::endl;
    }
}
