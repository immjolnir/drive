#pragma once

#include "a.h"
#include "b.h"

#include <ostream>

class MyClass {
    A a_;
    B* b_;

  public:
    MyClass();
    ~MyClass();
    
    void update();
    friend std::ostream& operator<<(std::ostream& out, const MyClass& obj);
};
