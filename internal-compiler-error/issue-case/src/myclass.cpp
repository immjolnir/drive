#include "myclass.h"

MyClass::MyClass() { b_ = new B(); }

MyClass::~MyClass() { delete b_; }

void MyClass::update() {
    b_->i = 1;
    b_->j = 2;
    b_->k = 3;
    auto& a = b_->a;
    a.i = 10;
    a.j = 10;
    a.k = 10;
}

std::ostream& operator<<(std::ostream& out, const MyClass& obj) {
    // clang-format off
    const auto& a = obj.b_->a;
    out << "MyClass[(b: " << obj.b_->i << ", " << obj.b_->j << ", " << obj.b_->k 
        << "; a: " << a.i << ", " << a.j << ", " << a.k << ")"
        << ", (a: " << obj.a_.i << ", " << obj.a_.j << ", " << obj.a_.k << ")]";
    // clang-format on
    return out;
}