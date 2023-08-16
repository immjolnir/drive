#pragma once

#include "a.h"

#include <ostream>

// In order to improve up compilation times, replace #include directives with forward declarations of types whenever
// possible.
// This can make the final file size a lot smaller!
class B;

class MyClass {
    A a_;
    B* b_;

  public:
    MyClass();
    ~MyClass();
    void update();
    friend std::ostream& operator<<(std::ostream& out, const MyClass& obj);
};
