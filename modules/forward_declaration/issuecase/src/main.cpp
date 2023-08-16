#include "myclass.h"

#include <iostream>
#include <string>

std::string s = "It has tens of thousands of lines, parsing it will cause cc1plus compiler crash.";

int main() {
    MyClass my;
    my.update();
    std::cout << my << std::endl;
}