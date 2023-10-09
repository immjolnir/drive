// No need to include it because this is to define the `extern` variable.
// #include "global_variables.h"

int i = 2;  // Add the definition for that variable declared with `extern`.

// Although these two variables are repeatedly defined here,
// their scope is limited to the file and will not be passed out.
// They are file variables.
int j = 10;
int k = 11;

/*
C++ programs are compiled one translation unit at a time (basically this means that each .cpp file is compiled
independently).

By default, variables that are not _const_, functions that are not _inline_ and _typedef_s have internal linkage:
this means that they are not visible to other translation units.

If other translation units refer to a symbol having internal linkage (after declaring it, which requires the keyword
_extern_ for variables), the linker won't be able to find them.

To explicitly require internal linkage use the keyword _static_ or, better, use unnamed namespaces.
*/
