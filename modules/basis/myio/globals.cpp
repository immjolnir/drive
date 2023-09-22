#include "io_impl.h"

namespace my
{
    typedef char fake_mystream[sizeof(mystream)] __attribute__((aligned(__alignof__(mystream))));

    fake_mystream mout;  // Only allocating space instead of construct it.
}  // namespace my