# enum

- https://www.linuxtopia.org/online_books/programming_books/thinking_in_c++/Chapter08_023.html

## The “enum hack” in old code

In older versions of C++, `static const` was not supported inside classes. This meant that const was useless for constant expressions inside classes. However, people still wanted to do this so a typical solution (usually referred to as the “enum hack”) was to use an untagged enum with no instances. An enumeration must have all its values established at compile time, it’s local to the class, and its values are available for constant expressions. Thus, you will commonly see:
```c++
//: C08:EnumHack.cpp
#include <iostream>
using namespace std;

class Bunch {
  enum { size = 1000 };
  int i[size];
};

int main() {
  cout << "sizeof(Bunch) = " << sizeof(Bunch) 
       << ", sizeof(i[1000]) = " 
       << sizeof(int[1000]) << endl;
}
```

The use of enum here is guaranteed to occupy no storage in the object, and the enumerators are all evaluated at compile time. You can also explicitly establish the values of the enumerators:
```c++
enum { one = 1, two = 2, three };
```

With integral enum types, the compiler will continue counting from the last value, so the enumerator `three` will get the value `3`.

In the StringStack.cpp example above, the line:

```c++
static const int size = 100;
```
would be instead:
```c++
enum { size = 100 };
```
Although you’ll often see the enum technique in legacy code, the static const feature was added to the language to solve just this problem. However, there is no overwhelming reason that you must choose static const over the enum hack, and in this book the enum hack is used because it is supported by more compilers at the time this book was written.