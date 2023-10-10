#pragma once

#include <new>  // std::launder

namespace arrow
{
    namespace internal
    {

// defined in c++11 library
#if __cpp_lib_launder
        using std::launder;
#else
        template <class T>
        constexpr T* launder(T* p) noexcept {
            return p;
        }
#endif

    }  // namespace internal
}  // namespace arrow
