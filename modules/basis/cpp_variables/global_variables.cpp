#include "global_variables.h"

int i = 2;  // Add the definition for that variable declared with `extern`.

/*
error: redefinition of 'int j'
    4 | int j = 2;
      |     ^
So, the global_varaibles.h has defined it.
*/
// int j = 2;

/*
 error: redefinition of 'int k'
    5 | int k = 2;
      |     ^
So, the global_varaibles.h has defined it.
*/
// int k = 2;