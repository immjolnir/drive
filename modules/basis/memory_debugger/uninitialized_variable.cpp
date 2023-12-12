#include <iostream>

// Disable "Warnings treated as errors"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

int main() {
    // error: 'x' is used uninitialized [-Werror=uninitialized]
    int x;

    if (x == 0) {
        std::cout << "X is zero" << std::endl;
    }
    return 0;
}

#pragma GCC diagnostic pop
