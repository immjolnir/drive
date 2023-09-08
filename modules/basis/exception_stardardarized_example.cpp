#include <stdexcept>

/*
* https://learn.microsoft.com/en-us/cpp/cpp/try-throw-and-catch-statements-cpp?view=msvc-170

To implement exception handling in C++, you use try, throw, and catch expressions.

- The code after the `try` clause is the guarded section of code.

- The `throw` expression throws—that is, raises—an exception.

- The code block after the `catch` clause is the exception handler.

This is the handler that catches the exception that's thrown if the types in the throw and catch expressions are
compatible. For a list of rules that govern type-matching in catch blocks,

  - If the catch statement specifies an ellipsis (...) instead of a type, the catch block handles every type of
exception. Because catch blocks are processed in program order to find a matching type, an ellipsis handler must be the
last handler for the associated try block. Use catch(...) with caution; don't allow a program to continue unless the
catch block knows how to handle the specific exception that is caught. Typically, a catch(...) block is used to log
errors and perform special cleanup before program execution is stopped.



In most cases, we recommend that you use the `std::exception` class or one of the derived classes that are
defined in the standard library.

If one of those isn't appropriate, we recommend that you derive your own exception class from std::exception.

- https://learn.microsoft.com/en-us/cpp/cpp/how-catch-blocks-are-evaluated-cpp?view=msvc-170

The order in which catch handlers appear is significant, because handlers for a given try block are examined in order of
their appearance.

For example, it is an error to place the handler for a base class before the handler for a derived class.

After a matching catch handler is found, subsequent handlers are not examined.
As a result, an `ellipsis catch handler` must be the last handler for its try block.
*/

#include <exception>
#include <iostream>

#include <gtest/gtest.h>

/*
class FatalException : public std::exception {
  public:
    FatalException(const char* filename, int line, const std::string& message)
      : filename_(filename), line_(line), message_(message) {}

    virtual ~FatalException() throw();

    virtual const char* what() const throw();

    const char* filename() const { return filename_; }

    int line() const { return line_; }

    const std::string& message() const { return message_; }

  private:
    const char* filename_;
    const int line_;
    const std::string message_;
};
*/

class networkIOException : public std::exception {
  public:
    networkIOException(const std::string& message) : message_(message) {}

    virtual ~networkIOException() throw() {}

    virtual const char* what() const throw() { return "networkIOError"; }

    const std::string& message() const { return message_; }

  private:
    const std::string message_;
};

class myDataFormatException : public std::exception {
  public:
    myDataFormatException(const char* message) : message_(message) {}

    virtual ~myDataFormatException() throw() {}

    virtual const char* what() const throw() { return message_; }

  private:
    const char* message_;
};

struct MyData {};

class ExceptionHandlingTest : public ::testing::Test {
  public:
    bool IOSuccess = true;
    bool readError = false;

    bool raise_range_error = false;
    const char* em = "Base";

    bool raise_noexception_type = false;
    int age = 0;

    MyData md;

    virtual void SetUp() {}

    virtual void TearDown() {}

    // The following syntax shows a throw expression
    MyData GetNetworkResource() {
        if (IOSuccess == false) throw networkIOException("Unable to connect");
        if (readError) throw myDataFormatException("Format error");
        if (raise_range_error) throw std::range_error(em);
        if (raise_noexception_type) {
            age = 15;
            throw(age);
        }
        static const MyData data;
        return data;
    }
};

TEST_F(ExceptionHandlingTest, network_io_exception) {
    IOSuccess = false;

    try {
        // Code that could throw an exception
        md = GetNetworkResource();
    } catch (const networkIOException& e) {
        // Code that executes when an exception of type
        // networkIOException is thrown in the try block
        // ...
        // Log error message in the exception object
        std::cerr << e.what();
        EXPECT_EQ(e.what(), std::string("networkIOError"));
        EXPECT_EQ("Unable to connect", e.message());
    } catch (const myDataFormatException& e) {
        FAIL() << "Expected networkIOException 1";
    } catch (const std::exception& e) {
        FAIL() << "Expected networkIOException 2";
    } catch (...) {
        FAIL() << "Expected networkIOException 3";
    }
}

TEST_F(ExceptionHandlingTest, mydata_format_exception) {
    readError = true;
    try {
        // Code that could throw an exception
        md = GetNetworkResource();
    } catch (const networkIOException& e) {
        std::cerr << e.what();
        FAIL() << "Expected myDataFormatException 1";
    } catch (const myDataFormatException& e) {
        // Code that handles another exception type
        std::cerr << e.what();
        EXPECT_EQ(e.what(), std::string("Format error"));
    } catch (const std::exception& e) {
        FAIL() << "Expected myDataFormatException 2";
    } catch (...) {
        FAIL() << "Expected myDataFormatException 3";
    }
}

TEST_F(ExceptionHandlingTest, range_error) {
    raise_range_error = true;

    try {
        // Code that could throw an exception
        md = GetNetworkResource();
    } catch (const networkIOException& e) {
        FAIL() << "Expected Base Exception 1";
    } catch (const myDataFormatException& e) {
        std::cerr << e.what();
        FAIL() << "Expected Base Exception 2";
    } catch (const std::exception& e) {
        std::cerr << "base handler] " << e.what() << '\n';  // or whatever
        EXPECT_EQ(e.what(), std::string("Base"));
    } catch (...) {
        FAIL() << "Expected Base Exception 3";
    }
}

TEST_F(ExceptionHandlingTest, no_exception_type) {
    /*
    当 thow 一个 非继承自 std::exception 的 value 时，才能触发 ellipsis.
    */
    raise_noexception_type = true;

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
        std::cerr << "ellipsis handler] who knows!" << std::endl;
        EXPECT_EQ(15, age);
    }
}

TEST_F(ExceptionHandlingTest, handle_opaque_exception) {
    /*
    当 thow 一个 非继承自 std::exception 的 value 时，才能触发 ellipsis.
    */
    raise_noexception_type = true;
    std::exception_ptr eptr;

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

    try {
        if (eptr) std::rethrow_exception(eptr);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: '" << e.what() << "'\n";
    }
    // unknown file
    // Unknown C++ exception thrown in the test body.
}