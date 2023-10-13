#pragma once

// Copy
// apache/arrow/cpp/src/arrow/util/aligned_storage.h
//
#include <cstring>
#include <type_traits>
#include <utility>

#include "arrow/util/launder.h"
#include "arrow/util/macros.h"

namespace arrow
{
    namespace internal
    {

        template <typename T>
        class AlignedStorage {
          public:
            static constexpr bool can_memcpy = std::is_trivial<T>::value;

            constexpr T* get() noexcept { return arrow::internal::launder(reinterpret_cast<T*>(&data_)); }

            constexpr const T* get() const noexcept {
                // Use fully qualified name to avoid ambiguities with MSVC (ARROW-14800)
                return arrow::internal::launder(reinterpret_cast<const T*>(&data_));
            }

            void destroy() noexcept {
                if (!std::is_trivially_destructible<T>::value) {
                    get()->~T();
                }
            }

            template <typename... A>
            void construct(A&&... args) noexcept {
                // placement new operator
                new (&data_) T(std::forward<A>(args)...);  // It must have a constructor: T(args).
            }

            template <typename V>
            void assign(V&& v) noexcept {
                *get() = std::forward<V>(v);
            }

            void move_construct(AlignedStorage* other) noexcept {
                // placement new operator
                new (&data_) T(std::move(*other->get()));
            }

            void move_assign(AlignedStorage* other) noexcept { *get() = std::move(*other->get()); }

            template <bool CanMemcpy = can_memcpy>
            static typename std::enable_if<CanMemcpy>::type move_construct_several(AlignedStorage* ARROW_RESTRICT src,
                                                                                   AlignedStorage* ARROW_RESTRICT dest,
                                                                                   size_t n,
                                                                                   size_t memcpy_length) noexcept {
                memcpy(dest->get(), src->get(), memcpy_length * sizeof(T));
            }

            static void move_construct_several(AlignedStorage* ARROW_RESTRICT src, AlignedStorage* ARROW_RESTRICT dest,
                                               size_t n) noexcept {
                move_construct_several(src, dest, n, n);
            }

            static void destroy_several(AlignedStorage* p, size_t n) noexcept {
                if (!std::is_trivially_destructible<T>::value) {
                    for (size_t i = 0; i < n; ++i) {
                        p[i].destroy();
                    }
                }
            }

          private:
#if !defined(__clang__) && defined(__GNUC__) && defined(__i386__)
            // Workaround for GCC bug on i386:
            //   alignof(int64 | float64) can give different results depending on the
            //   compilation context, leading to internal ABI mismatch manifesting
            //   in incorrect propagation of Result<int64 | float64> between
            //   compilation units.
            // (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=88115)
            static constexpr size_t alignment() {
                if (std::is_integral_v<T> && sizeof(T) == 8) {
                    return 4;
                } else if (std::is_floating_point_v<T> && sizeof(T) == 8) {
                    return 4;
                }
                return alignof(T);
            }

            typename std::aligned_storage<sizeof(T), alignment()>::type data_;
#else
            typename std::aligned_storage<sizeof(T), alignof(T)>::type data_;
#endif
        };
    }  // namespace internal
}  // namespace arrow