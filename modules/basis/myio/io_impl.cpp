#include "io_impl.h"

#include <iostream>

namespace my
{
    _Atomic_word ios_base::Init::_S_refcount;
    bool ios_base::Init::_S_synced_with_stdio = true;

    mystream::mystream() { std::cout << "Calling my::mystream::mystream().But do not call it!" << std::endl; }

    mystream::mystream(std::stringstream* buf) {
        _buf = buf;
        std::cout << "Calling my::mystream::mystream(std::stringstream*)" << std::endl;
    }

    mystream::~mystream() {
        std::cout << "Calling my::mystream::~mystream(). But actually, you won't see it." << std::endl;
    }

    extern mystream mout;

    ios_base::Init::Init() {
        std::cout << "Calling ios_base::Init::Init()" << std::endl;
        if (__gnu_cxx::__exchange_and_add_dispatch(&_S_refcount, 1) == 0) {
            // Standard streams default to synced with "C" operations.
            _S_synced_with_stdio = true;
            static std::stringstream buf_cout_sync;
            
            // Placement new allows you to construct an object in memory that's already allocated.
            new (&mout) mystream(&buf_cout_sync);
            __gnu_cxx::__atomic_add_dispatch(&_S_refcount, 1);
        }
    }

    ios_base::Init::~Init() { std::cout << "Calling ios_base::Init::~Init()" << std::endl; }

}  // namespace my