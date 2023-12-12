#include <iostream>
#include <string>

// What are the initial values, if any, of each of the following variables?
// C++ Primer, Fifth Edition P75
//
std::string global_str;
int global_int;

int main() {
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    int local_int;
    std::string local_str;

    std::cout << global_str << std::endl;
    std::cout << global_int << std::endl;

    std::cout << local_int << std::endl;  // undefined value.
    std::cout << local_str << std::endl;
}

/*
 *
$ ./a.out

0
32714


 */
