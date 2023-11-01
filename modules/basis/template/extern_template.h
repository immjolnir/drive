/**
 * https://stackoverflow.com/questions/8130602/using-extern-template-c11-to-avoid-instantiation
 *  Copy from: src/google/protobuf/message_lite.h
 */
#include <assert.h>
#include <string.h>
#include <ostream>
#include <string>

namespace google {
namespace protobuf {
// StringPiece has *two* size types.
// StringPiece::size_type
//   is unsigned
//   is 32 bits in LP32, 64 bits in LP64, 64 bits in LLP64
//   no future changes intended
// stringpiece_ssize_type
//   is signed
//   is 32 bits in LP32, 64 bits in LP64, 64 bits in LLP64
//   future changes intended: http://go/64BitStringPiece
//
typedef std::string::difference_type stringpiece_ssize_type;

class StringPiece {
  public:
    typedef size_t size_type;

    // We provide non-explicit singleton constructors so users can pass
    // in a "const char*" or a "string" wherever a "StringPiece" is
    // expected.
    //
    // Style guide exception granted:
    // http://goto/style-guide-exception-20978288
    StringPiece() : ptr_(nullptr), length_(0) {}

    StringPiece(const char* str)  // NOLINT(runtime/explicit)
      : ptr_(str), length_(0) {
        if (str != nullptr) {
            length_ = strlen(str);
        }
    }

    template <class Allocator>
    StringPiece(  // NOLINT(runtime/explicit)
      const std::basic_string<char, std::char_traits<char>, Allocator>& str)
      : ptr_(str.data()), length_(0) {
        length_ = str.size();
    }

    StringPiece(const char* offset, stringpiece_ssize_type len) : ptr_(offset), length_(len) { assert(len >= 0); }

    // data() may return a pointer to a buffer with embedded NULs, and the
    // returned buffer may or may not be null terminated.  Therefore it is
    // typically a mistake to pass data() to a routine that expects a NUL
    // terminated string.
    const char* data() const { return ptr_; }

    stringpiece_ssize_type size() const { return length_; }

    stringpiece_ssize_type length() const { return length_; }

    bool empty() const { return length_ == 0; }

    void clear() {
        ptr_ = nullptr;
        length_ = 0;
    }

    char operator[](stringpiece_ssize_type i) const {
        assert(0 <= i);
        assert(i < length_);
        return ptr_[i];
    }

    std::string as_string() const { return ToString(); }

    // We also define ToString() here, since many other string-like
    // interfaces name the routine that converts to a C++ string
    // "ToString", and it's confusing to have the method that does that
    // for a StringPiece be called "as_string()".  We also leave the
    // "as_string()" method defined here for existing code.
    std::string ToString() const {
        if (ptr_ == nullptr) return "";
        return std::string(data(), static_cast<size_type>(size()));
    }

    explicit operator std::string() const { return ToString(); }

    static const size_type npos;

  private:
    const char* ptr_;
    stringpiece_ssize_type length_;
};

// This large function is defined inline so that in a fairly common case where
// one of the arguments is a literal, the compiler can elide a lot of the
// following comparisons.
inline bool operator==(StringPiece x, StringPiece y) {
    stringpiece_ssize_type len = x.size();
    if (len != y.size()) {
        return false;
    }

    return x.data() == y.data() || len <= 0 || memcmp(x.data(), y.data(), static_cast<size_t>(len)) == 0;
}

inline bool operator!=(StringPiece x, StringPiece y) { return !(x == y); }

inline bool operator<(StringPiece x, StringPiece y) {
    const stringpiece_ssize_type min_size = x.size() < y.size() ? x.size() : y.size();
    const int r = memcmp(x.data(), y.data(), static_cast<size_t>(min_size));
    return (r < 0) || (r == 0 && x.size() < y.size());
}

inline bool operator>(StringPiece x, StringPiece y) { return y < x; }

inline bool operator<=(StringPiece x, StringPiece y) { return !(x > y); }

inline bool operator>=(StringPiece x, StringPiece y) { return !(x < y); }

// allow StringPiece to be logged
extern std::ostream& operator<<(std::ostream& o, StringPiece piece);

namespace internal {
template <bool alias>
bool MergeFromImpl(StringPiece input);

extern template bool MergeFromImpl<false>(StringPiece input);
extern template bool MergeFromImpl<true>(StringPiece input);

}  // namespace internal
}  // namespace protobuf
}  // namespace google
