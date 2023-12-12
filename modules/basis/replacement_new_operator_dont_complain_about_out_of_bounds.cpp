#include <iostream>
#include <new>

struct alignas(8) SimpleChar {
    SimpleChar(char c_) : c(c_) {}

    char c;
};

int main() {
    char slab[10] = {'\0'};  // it must be a multiple of SimpleChar's size. Here should be 8, 16, 24,...
    // call to 'SimpleChar::SimpleChar(char)' too
    //
// Disable "Warnings treated as errors"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wplacement-new="
    SimpleChar* c0 = new (slab) SimpleChar('a');
    std::cout << c0->c << std::endl;

    SimpleChar* c1 = new (slab + 8) SimpleChar('b');
    std::cout << c1->c << std::endl;

    SimpleChar* c2 =
      new (std::launder(reinterpret_cast<char*>(slab + 80))) SimpleChar('d');  // But how to detect the wrong usage?
    std::cout << c2->c << std::endl;                                           // d

    SimpleChar* c3 = new (slab + 80) SimpleChar('e');  // But how to detect the wrong usage?
    std::cout << c3->c << std::endl;                   // e
#pragma GCC diagnostic pop
}