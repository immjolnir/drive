#include <iostream>
#include <new>

struct alignas(8) SimpleChar {
    SimpleChar(char c_) : c(c_) {}

    char c;
};

int main() {
    char slab[10] = {'\0'};

    // call to 'SimpleChar::SimpleChar(char)' too
    SimpleChar* c0 = new (slab) SimpleChar('a');
    // clang-format off
    /*
    warning: placement new constructing an object of type 'SimpleChar' and size '8' in a region of type 'char [10]' and size '2' [-Wplacement-new=]
    22 |     SimpleChar* c1 = new (slab + 8) SimpleChar('b');
      |                           ~~~~~^~~
    10: note: at offset 8 from 'slab' declared here
   18 |     char slab[10] = {'\0'};
      |          ^~~~
    */
    // clang-format on
    SimpleChar* c1 = new (slab + 8) SimpleChar('b');
    std::cout << c0->c << std::endl;
    std::cout << c1->c << std::endl;

    // Unsafe but still safe
    // clang-format off
    /*
    warning: placement new constructing an object of type 'SimpleChar' and size '8' in a region of type 'char [10]' and size '0' [-Wplacement-new=]
    38 |     SimpleChar* c2 = new (slab + 80) SimpleChar('d');
    */
    // clang-format on

    SimpleChar* c2 =
      new (std::launder(reinterpret_cast<char*>(slab + 80))) SimpleChar('d');  // But how to detect the wrong usage?
    std::cout << c2->c << std::endl;                                           // d

    SimpleChar* c3 = new (slab + 80) SimpleChar('e');  // But how to detect the wrong usage?
    std::cout << c3->c << std::endl;                   // e

    return 0;
}