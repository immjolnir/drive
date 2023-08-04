// https://stackoverflow.com/questions/29883327/is-it-safe-to-remove-const-via-const-cast-and-invoke-a-non-const-function-that-d
#include <iostream>

struct Foo {
    void f() const  // doesn't modify the instance, although is not marked const
    {
        std::cout << "Foo::f()" << std::endl;
    }
};

struct Bar {
    void f(int modified) {
        m = modified;
        std::cout << "Foo::f() with m=" << m << std::endl;
    }

    int m = 0;
};

int main() {
    const Foo foo;
    const_cast<Foo&>(foo).f();  // is this safe? Yes

    const Bar bar;
    const_cast<Bar&>(bar).f(10);  // Is this safe? No
}