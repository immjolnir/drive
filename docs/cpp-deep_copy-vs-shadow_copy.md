# Mastering Copy Constructor in C++ - Shallow vs Deep Copy with Examples

- Introduction:
C++ is a popular programming language used for developing a variety of software applications. It is an object-oriented programming language that allows creating new objects from existing ones. Copy Constructor is an essential concept in C++ that enables developers to create new objects from an existing object.

- What is a Copy Constructor in C++?
A Copy Constructor is a special type of constructor that creates a new object by copying an existing object. It creates an exact copy of the original object, including all its data members and member functions.

- Shallow Copy Constructor
A Shallow Copy Constructor is a Copy Constructor that performs a shallow copy of the object. In a shallow copy, only the object's data members are copied, and any pointers to memory allocated dynamically are not copied. Instead, the new object points to the same memory location as the original object. This can lead to issues if the original object is modified, as the copied object will also be affected.

- Deep Copy Constructor
A Deep Copy Constructor is a Copy Constructor that performs a deep copy of the object. In a deep copy, not only the object's data members are copied, but any dynamically allocated memory is also copied to a new location. This ensures that the copied object is independent of the original object and can be modified without affecting the original object.


- What happens if I do not define a Copy Constructor in C++?

If you do not define a Copy Constructor in C++, the compiler will generate a default one for you.
This default constructor will perform a Shallow Copy of the object, which may lead to unexpected behavior if the object contains dynamically allocated memory or reference data members.

- Why doesn't C++ generate a deep copy constructor by default?

From ChatGPT:

> C++ does not generate a deep copy constructor by default because it would be inefficient and potentially dangerous. A deep copy constructor creates a new object that is an exact copy of the original, including all of its member variables. This can be very time-consuming and memory-intensive, especially if the object contains large amounts of data or complex data structures. Additionally, if the original object contains pointers to dynamically allocated memory, a deep copy constructor would also need to allocate new memory and copy the data, which could lead to memory leaks if not handled properly. To avoid these issues, C++ allows developers to create their own copy constructors and operators that are tailored to the specific needs of the class.

- What is the syntax for defining a Copy Constructor in C++?

```c++
ClassName::ClassName(const ClassName& obj) {
            // Copy the data members of obj to the new object
}
```

- How can I avoid using the Copy Constructor in C++?

You can avoid using the Copy Constructor in C++ by using pointers or references to objects instead of creating copies. This can improve performance and prevent errors that may occur with copying objects

- What is the difference between a Shallow Copy and a Deep Copy in C++?

A `Shallow Copy` in C++ copies only the addresses of the dynamically allocated memory in an object, while a Deep `Copy copies` the values of the memory, creating a new memory block for the copy.
`Shallow Copy` can result in objects sharing the same memory, while `Deep Copy` ensures each object has its own independent memory.


- Shadow copy example
```c++
#include <iostream>
using namespace std;

class MyClass {
    public:
        int *x;
        MyClass(int val) {
            x = new int(val);
        }
        MyClass(const MyClass &obj) {
            x = obj.x;
        }
        ~MyClass() {
            delete x;
        }
};
```

- Deep copy example
```c++
class MyClass {
    public:
        int *x;
        MyClass(int val) {
            x = new int(val);
        }
        MyClass(const MyClass &obj) {
            x = new int(*obj.x);
        }
        ~MyClass() {
            delete x;
        }
};
```

- Difference between Copy Constructor and Assignment Operator

While both `Copy Constructor` and ``Assignment Operator`` can be used to copy objects, they have some differences. The `Copy Constructor` creates a new object by copying an existing object, while the `Assignment Operator` assigns the value of an existing object to another object. Here are some key differences between the two:

  - The `Copy Constructor` is called when a new object is created from an existing object, while the `Assignment Operator` is called when an existing object is assigned a new value.
  
  - The `Copy Constructor` takes a reference to an object of the same class as a parameter, while the `Assignment Operator` takes a reference to an object of the same class as a parameter and returns a reference to the assigned object.

  - The `Copy Constructor` creates a new object and allocates new memory for it, while the `Assignment Operator` modifies the existing object and may or may not allocate new memory for it.

  - The `Copy Constructor` is called only once during object initialization, while the `Assignment Operator` can be called multiple times during the lifetime of an object.


# [The Container concept](https://www.cs.helsinki.fi/u/tpkarkka/alglib/k06/lectures/containers.html)
Properties shared by all STL containers.

- default constructor

- copy constructor and assignment
  - deep copy [均是 deep copy]

- swap
  - a.swap(b) and swap(a, b)
  - constant time

- `==`, `!=`

  - content-based equality: equal elements in same order

- order comparisons

  - lexicographic order: first inequal elements determine the order
  ```c++
  vector<int> a, b;
  // a = [1, 2, 3]
  // b = [1, 3, 2]
  assert(a < b);
  ```

- `begin()`, `end()`

- `size()`, `empty()`, `max_size()`

- member types

  - value_type
  - reference (to the value type)
  - const_reference
  - iterator
  - const_iterator
  - difference_type (as with iterators)
  - size_type (often unsigned type, usually size_t)


In addition, a reversible container has the properties:

- `rbegin()`, `rend()`
- member types
  - reverse_iterator
  - const_reverse_iterator

正因为 container都是 deep copy, c++ 才可以提供 string_view 等