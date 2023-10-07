#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

#include <string.h>  // memcmp, compare memory areas

namespace arrow
{
    // arrow/cpp/src/arrow/buffer.h
    class Buffer {
      public:
        /// \brief Construct from buffer and size without copying memory
        ///
        /// \param[in] data a memory buffer
        /// \param[in] size buffer size
        ///
        /// \note The passed memory must be kept alive through some other means
        Buffer(const uint8_t* data, int64_t size) : data_(data), size_(size), capacity_(size) {}

        /// \brief View buffer contents as a std::string_view
        /// \return std::string_view
        explicit operator std::string_view() const {
            return {reinterpret_cast<const char*>(data_), static_cast<size_t>(size_)};
        }

        uint8_t operator[](std::size_t i) const { return data_[i]; }

        /// Copy a section of the buffer into a new Buffer.
        // cpp/src/arrow/result.h
        // Result<std::shared_ptr<Buffer>> CopySlice(const int64_t start, const int64_t nbytes) const {}

        // [C++] Implement zero-copy Slice method on arrow::Buffer that retains reference to parent
        // https://github.com/apache/arrow/issues/15767
        /// \defgroup buffer-slicing-functions Functions for slicing buffers
        ///
        /// @{

        /// \brief Construct a view on a buffer at the given offset and length.
        ///
        /// This function cannot fail and does not check for errors (except in debug builds)
        // static inline std::shared_ptr<Buffer> SliceBuffer(const std::shared_ptr<Buffer>& buffer, const int64_t
        // offset,
        //                                                   const int64_t length) {
        //     return std::make_shared<Buffer>(buffer, offset, length);
        // }

        inline std::shared_ptr<Buffer> slice(const int64_t offset, const int64_t length) {
            return std::make_shared<Buffer>(this->data_ + offset, length);
        }

        /// \brief Return a pointer to the buffer's data cast to a specific type
        ///
        /// The buffer has to be a CPU buffer (`is_cpu()` is true).
        /// Otherwise, an assertion may be thrown or a null pointer may be returned.
        template <typename T>
        const T* data_as() const {
            return reinterpret_cast<const T*>(data_);
        }

        /// \brief Return the device address of the buffer's data
        uintptr_t address() const { return reinterpret_cast<uintptr_t>(data_); }

        /// \brief Return the buffer's size in bytes
        int64_t size() const { return size_; }

        /// \brief Return the buffer's capacity (number of allocated bytes)
        int64_t capacity() const { return capacity_; }

        bool Equals(const Buffer& other) const {
            return this == &other ||
                   (size_ == other.size_ &&
                    (data_ == other.data_ || !memcmp(data_, other.data_, static_cast<size_t>(size_))));
        }

      protected:
        const uint8_t* data_;
        int64_t size_;
        int64_t capacity_;

      private:
    };
}  // namespace arrow