#include <iostream>
#include <new>
#include <type_traits>

struct alignas(8) SimpleChar {
    SimpleChar(char c_) : c(c_) {}

    char c;
};

int main() {
    std::aligned_storage<sizeof(SimpleChar), alignof(SimpleChar)>::type slab;

    SimpleChar* p = reinterpret_cast<SimpleChar*>(&slab);

    SimpleChar* c0 = new (p) SimpleChar('a');
    std::cout << c0->c << std::endl;

    SimpleChar* c1 = new (p + 8) SimpleChar('b');
    std::cout << c1->c << std::endl;

    SimpleChar* c2 =
      new (std::launder(reinterpret_cast<char*>(p + 80))) SimpleChar('d');  // But how to detect the wrong usage?
    std::cout << c2->c << std::endl;                                        // d

    SimpleChar* c3 = new (p + 80) SimpleChar('e');  // But how to detect the wrong usage?
    std::cout << c3->c << std::endl;                // e
}