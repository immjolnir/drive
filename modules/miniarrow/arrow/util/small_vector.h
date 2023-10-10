#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

#include "arrow/util/aligned_storage.h"

namespace arrow
{
    namespace internal
    {
        // https://stackoverflow.com/questions/8190879/what-is-a-non-trivial-destructor-in-c
        /*
         * The rule is very straight-forward: Does your class have an explicit destructor?
         * If yes, you're non-trivial.
         * If no, check each non-static member object; if any of them are non-trivial, then you're non-trivial.
         */
        template <typename T, size_t N, bool NonTrivialDestructor>
        struct StaticVectorStorageBase {
            using storage_type = AlignedStorage<T>;

            storage_type static_data_[N];
            size_t size_ = 0;

            void destroy() noexcept {}
        };

        template <typename T, size_t N>
        struct StaticVectorStorageBase<T, N, true> {
            using storage_type = AlignedStorage<T>;

            storage_type static_data_[N];
            size_t size_ = 0;

            ~StaticVectorStorageBase() noexcept { destroy(); }

            void destroy() noexcept { storage_type::destroy_several(static_data_, size_); }
        };

        template <typename T, size_t N, bool D = !std::is_trivially_destructible<T>::value>
        struct StaticVectorStorage : public StaticVectorStorageBase<T, N, D> {
            using Base = StaticVectorStorageBase<T, N, D>;
            using typename Base::storage_type;

            using Base::size_;
            using Base::static_data_;

            StaticVectorStorage() noexcept = default;

            constexpr storage_type* storage_ptr() { return static_data_; }

            constexpr const storage_type* const_storage_ptr() const { return static_data_; }

            // Adjust storage size, but don't initialize any objects
            void bump_size(size_t addend) {
                assert(size_ + addend <= N);
                size_ += addend;
            }

            void ensure_capacity(size_t min_capacity) { assert(min_capacity <= N); }

            // Adjust storage size, but don't destroy any objects
            void reduce_size(size_t reduce_by) {
                assert(reduce_by <= size_);
                size_ -= reduce_by;
            }

            // Move objects from another storage, but don't destroy any objects currently
            // stored in *this.
            // You need to call destroy() first if necessary (e.g. in a
            // move assignment operator).
            void move_construct(StaticVectorStorage&& other) noexcept {
                size_ = other.size_;
                if (size_ != 0) {
                    // Use a compile-time memcpy size (N) for trivial types
                    storage_type::move_construct_several(other.static_data_, static_data_, size_, N);
                }
            }

            constexpr size_t capacity() const { return N; }

            void reserve(size_t n) {}

            void clear() {
                storage_type::destroy_several(static_data_, size_);
                size_ = 0;
            }
        };

        template <typename T, size_t N>
        struct SmallVectorStorage {
            using storage_type = AlignedStorage<T>;

            storage_type static_data_[N];
            size_t size_ = 0;
            storage_type* data_ = static_data_;
            size_t dynamic_capacity_ = 0;

            SmallVectorStorage() noexcept = default;

            ~SmallVectorStorage() { destroy(); }

            constexpr storage_type* storage_ptr() { return data_; }

            constexpr const storage_type* const_storage_ptr() const { return data_; }

            void bump_size(size_t addend) {
                const size_t new_size = size_ + addend;
                ensure_capacity(new_size);
                size_ = new_size;
            }

            void ensure_capacity(size_t min_capacity) {
                if (dynamic_capacity_) {
                    // Grow dynamic storage if necessary
                    if (min_capacity > dynamic_capacity_) {
                        size_t new_capacity = std::max(dynamic_capacity_ * 2, min_capacity);
                        reallocate_dynamic(new_capacity);
                    }
                } else if (min_capacity > N) {
                    switch_to_dynamic(min_capacity);
                }
            }

            void reduce_size(size_t reduce_by) {
                assert(reduce_by <= size_);
                size_ -= reduce_by;
            }

            void destroy() noexcept {
                storage_type::destroy_several(data_, size_);
                if (dynamic_capacity_) {
                    delete[] data_;
                }
            }

            void move_construct(SmallVectorStorage&& other) noexcept {
                size_ = other.size_;
                dynamic_capacity_ = other.dynamic_capacity_;
                if (dynamic_capacity_) {
                    data_ = other.data_;
                    other.data_ = other.static_data_;
                    other.dynamic_capacity_ = 0;
                    other.size_ = 0;
                } else if (size_ != 0) {
                    // Use a compile-time memcpy size (N) for trivial types
                    storage_type::move_construct_several(other.static_data_, static_data_, size_, N);
                }
            }

            constexpr size_t capacity() const { return dynamic_capacity_ ? dynamic_capacity_ : N; }

            void reserve(size_t n) {
                if (dynamic_capacity_) {
                    if (n > dynamic_capacity_) {
                        reallocate_dynamic(n);
                    }
                } else if (n > N) {
                    switch_to_dynamic(n);
                }
            }

            void clear() {
                storage_type::destroy_several(data_, size_);
                size_ = 0;
            }

          private:
            void switch_to_dynamic(size_t new_capacity) {
                dynamic_capacity_ = new_capacity;
                data_ = new storage_type[new_capacity];
                storage_type::move_construct_several_and_destroy_source(static_data_, data_, size_);
            }

            void reallocate_dynamic(size_t new_capacity) {
                assert(new_capacity >= size_);
                auto new_data = new storage_type[new_capacity];
                storage_type::move_construct_several_and_destroy_source(data_, new_data, size_);
                delete[] data_;
                dynamic_capacity_ = new_capacity;
                data_ = new_data;
            }
        };

        template <typename T, size_t N, typename Storage>
        class StaticVectorImpl {
          private:
            Storage storage_;

            T* data_ptr() { return storage_.storage_ptr()->get(); }

            constexpr const T* const_data_ptr() const { return storage_.const_storage_ptr()->get(); }

          public:
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            using iterator = T*;
            using const_iterator = const T*;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            constexpr StaticVectorImpl() noexcept = default;

            // Move and copy constructors
            StaticVectorImpl(StaticVectorImpl&& other) noexcept { storage_.move_construct(std::move(other.storage_)); }

            // Constructing from count and optional initialization value
            explicit StaticVectorImpl(size_t count) {
                storage_.bump_size(count);
                auto* p = storage_.storage_ptr();
                for (size_t i = 0; i < count; ++i) {
                    p[i].construct();
                }
            }

            StaticVectorImpl(size_t count, const T& value) {
                storage_.bump_size(count);
                auto* p = storage_.storage_ptr();
                for (size_t i = 0; i < count; ++i) {
                    p[i].construct(value);
                }
            }

            StaticVectorImpl(std::initializer_list<T> values) {
                storage_.bump_size(values.size());
                auto* p = storage_.storage_ptr();
                for (auto&& v : values) {
                    // Unfortunately, cannot move initializer values
                    p++->construct(v);
                }
            }

            // Size inspection
            constexpr bool empty() const { return storage_.size_ == 0; }

            constexpr size_t size() const { return storage_.size_; }

            constexpr size_t capacity() const { return storage_.capacity(); }

            // Data access
            T& operator[](size_t i) { return data_ptr()[i]; }

            constexpr const T& operator[](size_t i) const { return const_data_ptr()[i]; }

            T& front() { return data_ptr()[0]; }

            constexpr const T& front() const { return const_data_ptr()[0]; }

            T& back() { return data_ptr()[storage_.size_ - 1]; }

            constexpr const T& back() const { return const_data_ptr()[storage_.size_ - 1]; }

            // Iterators
            iterator begin() { return iterator(data_ptr()); }

            constexpr const_iterator begin() const { return const_iterator(const_data_ptr()); }

            iterator end() { return iterator(data_ptr() + storage_.size_); }

            constexpr const_iterator end() const { return const_iterator(const_data_ptr() + storage_.size_); }

            // Mutations
            void reserve(size_t n) { storage_.reserve(n); }

            void clear() { storage_.clear(); }

            void push_back(const T& value) {
                storage_.bump_size(1);
                storage_.storage_ptr()[storage_.size_ - 1].construct(value);
            }

            void push_back(T&& value) {
                storage_.bump_size(1);
                storage_.storage_ptr()[storage_.size_ - 1].construct(std::move(value));
            }

            template <typename... Args>
            void emplace_back(Args&&... args) {
                storage_.bump_size(1);
                storage_.storage_ptr()[storage_.size_ - 1].construct(std::forward<Args>(args)...);
            }
        };
    }  // namespace internal

    template <typename T, size_t N>
    using StaticVector = internal::StaticVectorImpl<T, N, internal::StaticVectorStorage<T, N>>;

    template <typename T, size_t N>
    using SmallVector = internal::StaticVectorImpl<T, N, internal::SmallVectorStorage<T, N>>;
}  // namespace arrow