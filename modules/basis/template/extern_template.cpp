#include "extern_template.h"

// #include <glog/logging.h>
// #include <google/protobuf/stubs/logging.h>  // for GOOGLE_CHECK

namespace google {
namespace protobuf {
std::ostream& operator<<(std::ostream& o, StringPiece piece) {
    // conversion to 'std::streamsize' {aka 'long int'} from 'size_t' {aka 'long unsigned int'} may change the sign of
    // the result [-Werror=sign-conversion]
    o.write(piece.data(), static_cast<std::streamsize>(piece.size()));
    return o;
}

const StringPiece::size_type StringPiece::npos = size_type(-1);

namespace internal {
// src/google/protobuf/stubs/strutil.h
inline bool ascii_isupper(char c) { return c >= 'A' && c <= 'Z'; }

inline char ascii_tolower(char c) { return ascii_isupper(c) ? c + ('a' - 'A') : c; }

static int memcasecmp(const char* s1, const char* s2, size_t len) {
    // ASCII code is 1 ~ 127
    const char* us1 = reinterpret_cast<const char*>(s1);
    const char* us2 = reinterpret_cast<const char*>(s2);

    for (int i = 0; i < len; i++) {
        const int diff = static_cast<int>(static_cast<char>(ascii_tolower(us1[i]))) -
                         static_cast<int>(static_cast<char>(ascii_tolower(us2[i])));
        if (diff != 0) return diff;
    }
    return 0;
}

inline bool CaseEqual(StringPiece s1, StringPiece s2) {
    if (s1.size() != s2.size()) return false;
    return memcasecmp(s1.data(), s2.data(), s1.size()) == 0;
}

bool safe_strtob(StringPiece str, bool* value) {
    // GOOGLE_CHECK(value != nullptr) << "nullptr output boolean given.";

    if (CaseEqual(str, "true") || CaseEqual(str, "t") || CaseEqual(str, "yes") || CaseEqual(str, "y") ||
        CaseEqual(str, "1")) {
        *value = true;
        return true;
    }
    if (CaseEqual(str, "false") || CaseEqual(str, "f") || CaseEqual(str, "no") || CaseEqual(str, "n") ||
        CaseEqual(str, "0")) {
        *value = false;
        return true;
    }
    return false;
}

template <bool aliasing>
bool MergeFromImpl(StringPiece input) {
    // when input contains the same boolean value as the aliasing, returns true; else return false;
    // aliasing = true, return ture when input has a "true", "t" or "yes"...
    // aliasing = false, return true when input has a "false", "f" or "no"...
    // It works like __expect
    bool value;
    if (safe_strtob(input, &value)) {
        return aliasing == value;
    } else {
        return false;
    }
}

template bool MergeFromImpl<false>(StringPiece input);
template bool MergeFromImpl<true>(StringPiece input);
}  // namespace internal
}  // namespace protobuf
}  // namespace google