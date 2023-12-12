#pragma once

extern int i;  // declares but does not define i
int j;         // declares and defines j
/*
warning: 'k' initialized and declared 'extern'
    5 | extern int k = 3;
      |            ^
*/
extern int k;  // just declare it.
