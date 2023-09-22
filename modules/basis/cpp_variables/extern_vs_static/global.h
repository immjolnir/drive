#pragma once

extern int i;  // declaration only
static int j;  // declaration & definition

static int k = i;  // is it Safe?
