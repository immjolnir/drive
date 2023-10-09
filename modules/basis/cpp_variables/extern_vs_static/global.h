#pragma once

extern int i;  // declaration only
static int j;  // declaration & definition

static int k = i;  // is it Safe?
// This trick comes from the declaration, definition and initialization of _std::cout_.
