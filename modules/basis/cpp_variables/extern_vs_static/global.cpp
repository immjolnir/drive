#include "global.h"

int i = 1;

// 1: error: 'j' does not name a type
// j = 2;

// 2: error: redefinition of 'int j'
// int j = 2;

// 3. error: redefinition of 'int j'
// static int j = 2;

// By now, we cannot define a static variable again.