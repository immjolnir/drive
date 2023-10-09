* Variables

A variable provides us with named storage that our programs can manipulate. Each
variable in C++ has a type. The type determines the size and layout of the variable’s
memory, the range of values that can be stored within that memory, and the set of
operations that can be applied to the variable. C++ programmers tend to refer to
variables as “variables” or “objects” interchangeably.

* variable defintion

A simple variable definition consists of a type specifier, followed by a list of one or
more variable names separated by commas, and ends with a semicolon. Each name in
the list has the type defined by the type specifier. A definition may (optionally) provide
an initial value for one or more of the names it defines:

    int sum=0, value, units_sold = 0;
    std::string book("0-201-78345-X"); // book initialized from string literal

what’s useful to know is that a string is a type that represents a variable-length sequence of characters.

* What is an Object?
C++ programmers tend to be cavalier in their use of the term object. Most
generally, an object is a region of memory that can contain data and has a
type.

An object that is initialized gets the specified value at the moment it is created.

* Initialization vs Assigenment

Initialization in C++ is a surprisingly complicated topic and one we will return to
again and again. Many programmers are confused by the use of the = symbol to
initialize a variable. It is tempting to think of initialization as a form of assignment, but
initialization and assignment are different operations in C++.

Initialization is not assignment. Initialization happens when a variable is given
a value when it is created. Assignment obliterates an object’s current value
and replaces that value with a new one.

    int units_solde = 0;
    int units_solde = {0};
    int units_solde{0};
    int units_solde(0);

The generalized use of curly braces for initialization was introduced as part of the
new standard. This form of initialization previously had been allowed only in more
restricted ways. this form of initialization is referred to as list initialization. Braced lists of initializers can now
be used whenever we initialize an object and in some cases when we assign a new value to an object.


When used with variables of built-in type, this form of initialization has one
important property: The compiler will not let us list initialize variables of built-in type if
the initializer might lead to the loss of information:

    long double ld = 3.1415926536;
    int a{ld}, b = {ld}; // error: narrowing conversion required
    int c(ld), d = ld;   // ok: but value will be truncated

* Default initialization

When we define a variable without a initializer, the variable is default initialized.

The value of an object of built-in type that is not explicitly initialized depends on
where it is defined. Variables defined outside any function body are initialized to zero.
With one exception, variables of built-in type defined inside a function are uninitialized.
The value of an uninitialized variable of built-in type is undefined.

It is an error to copy or otherwise try to access the value of a variable whose value is undefined.

Uninitialized objects of built-in type defined inside a function body have
undefined value. Objects of class type that we do not explicitly initialize have
a value that is defined by the class.

* Variable Declarations and Definitions
  - separate compilation
  Separate compilation lets us split our programs into several files, each of which can be compiled independently


Caution: Uninitialized Variables Cause Run-Time Problems
An uninitialized variable has an indeterminate value. Trying to use the value
of an uninitialized variable is an error that is often hard to debug. Moreover,
the compiler is not required to detect such errors, although most will warn
about at least some uses of uninitialized variables.

Tip
We recommend initializing every object of built-in type. It is not always
necessary, but it is easier and safer to provide an initializer until you can
be certain it is safe to omit the initializer.


To support separate compilation, C++ distinguishes between declarations and
definitions. A declaration makes a name known to the program. A file that wants to
use a name defined elsewhere includes a declaration for that name. A definition
creates the associated entity.

A variable declaration specifies the type and name of a variable. A variable definition
is a declaration. In addition to specifying the name and type, a definition also allocates
storage and may provide the variable with an initial value.

* extern

To obtain a declaration that is not also a definition, we add the extern keyword
and may not provide an explicit initializer:
    extern int i; // declares but does not define i
    int j; // declares and defines j

Any declaration that includes an explicit initializer is a definition. We can provide an
initializer on a variable defined as extern, but doing so overrides the extern. An
extern that has an initializer is a definition:
    extern double pi = 3.1416; // definition

It is an error to provide an initializer on an extern inside a function.

Note: Variables must be defined exactly once but can be declared many times.

The distinction between a declaration and a definition may seem obscure at this
point but is actually important. To use a variable in more than one file requires
declarations that are separate from the variable’s definition. To use the same variable
in multiple files, we must define that variable in one—and only one—file. Other files
that use that variable must declare—but not define—that variable.

* Static Typing
C++ is statically typed language, which means that types are checked at compile time. The process by which
types are checked is refered to as type checking.

## Variable Scope

The scope of a variable refers to the extent of the block of code in which the variable can be accessed, modified, and worked with. There are mainly two kinds of variable scopes.

The scope of a variable is the area of the program where the variable is valid.
- Global variables
- Local variables

## Global Variables

A global variable is valid from the point it is declared to the end of the program. 

A global variable can be accessed from anywhere in the entire program. It is usually declared at the top or start of the program outside of all blocks and functions of the program. It can be accessed from any function without any error.

- Definition of C++ Global Variable

A global variable can be accessed from anywhere in the entire program. It is usually declared at the top or start of the program outside of all blocks and functions of the program. It can be accessed from any function without any error.

- How to Access a Global Variable when There is a Local Variable with the Same Name?
So how do we access the global variable when there exists a local variable with the same name. To do that, we use the scope resolution operator as shown below.

```c++
#include<iostream>
using namespace std;

// Global variable  
int a = 3; 

int main()
{
    // Local variable   
    int a = 7;
    cout << "Value of global variable a is " << ::a<<endl;
    cout<< "Value of local variable a is " << a; 
}
```

- Working of C++ Global Variables
However, global variables are declared at the top of the program outside any function or block of code. These variables can be accessed from within any function of the block of code. They can be accessed and modified anywhere in the program without any compilation error.


## Local Variables

A local variable's scope is limited to the block where it is declared and cannot be accessed (set or read) outside that block. A block is a section of code enclosed in curly braces (`{ }`).

It is possible to declare a local variable with the same name as a global variable.

- In case both local and global variables are present with the same name, the compiler gives preference to the local variable when it is called by any method.

- In case both local and global variables are present with the same name, we use the scope resolution operator(`::`) to call the local variable.


See `module/basis/cpp_variables`.


* `extern` vs `static`
  extern int i;
  static int j;
