#pragma once

#include <new>  // std::launder

namespace arrow
{
    namespace internal
    {

#if __cpp_lib_launder
        using std::launder; // since c++17
#else
        template <class T>
        constexpr T* launder(T* p) noexcept {
            return p;
        }
#endif

    }  // namespace internal
}  // namespace arrow
