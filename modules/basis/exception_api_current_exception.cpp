#include <stdexcept>

/*
https://en.cppreference.com/w/cpp/error/current_exception

* rethrow_exception
* current_exception
* make_exception_ptr
*/

#include <exception>
#include <iostream>

#include <gtest/gtest.h>

void handle_eptr(std::exception_ptr eptr)  // passing by value is ok
{
    try {
        if (eptr) std::rethrow_exception(eptr);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: '" << e.what() << "'\n";
    }
}

TEST(exception, current_exception) {
    std::exception_ptr eptr;

    try {
        std::string().at(1);  // this generates a std::out_of_range
    } catch (...) {
        eptr = std::current_exception();  // capture
    }
    // destructor for std::out_of_range called here, when the eptr is destructed
    handle_eptr(eptr);
    // e.what();
}

/*
Creates an std::exception_ptr that holds a reference to a copy of e. This is done as if executing the following code:

try {
    throw e;
} catch(...) {
    return std::current_exception();
}
*/
TEST(exception, make_exception_ptr) {
    std::exception_ptr eptr;

    try {
        std::string().at(1);  // this generates a std::out_of_range
    } catch (const std::exception& e) {
        eptr = std::make_exception_ptr(e);  // capture
        std::cerr << "Logging: catch the exception: " << e.what() << std::endl;
    }
    // destructor for std::out_of_range called here, when the eptr is destructed
    handle_eptr(eptr);
}