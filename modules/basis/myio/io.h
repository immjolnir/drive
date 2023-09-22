#pragma once

#include "io_impl.h"

namespace my
{
    extern mystream mout;            // its dtor won't be called.
    static ios_base::Init __ioinit;  // its dtor will be called too.
}  // namespace my
