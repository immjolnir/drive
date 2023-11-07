#include <glog/logging.h>
#include <gtest/gtest.h>

#include <iterator>
#include <numeric>

// SerializeToString
/**
https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/message_lite.h#L383

    // Serialize the message and store it in the given string.  All required fields must be set.
  bool SerializeToString(std::string* output) const;

    // Like SerializeToString(), but allows missing required fields.
  bool SerializePartialToString(std::string* output) const;

 https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/message_lite.cc#L524C19-L524C36
*/
#define INT_MAX 128

namespace pb {

// src/google/protobuf/stubs/stl_util.h
// Inside Google, this function implements a horrible, disgusting hack in which
// we reach into the string's private implementation and resize it without
// initializing the new bytes.  In some cases doing this can significantly
// improve performance.  However, since it's totally non-portable it has no
// place in open source code.  Feel free to fill this function in with your
// own disgusting hack if you want the perf boost.
inline void STLStringResizeUninitialized(std::string* s, size_t new_size) { s->resize(new_size); }

// Return a mutable char* pointing to a string's internal buffer,
// which may not be null-terminated. Writing through this pointer will
// modify the string.
//
// string_as_array(&str)[i] is valid for 0 <= i < str.size() until the
// next call to a string method that invalidates iterators.
//
// As of 2006-04, there is no standard-blessed way of getting a
// mutable reference to a string's internal buffer. However, issue 530
// (http://www.open-std.org/JTC1/SC22/WG21/docs/lwg-active.html#530)
// proposes this as the method. According to Matt Austern, this should
// already work on all current implementations.
inline char* string_as_array(std::string* str) {
    // DO NOT USE const_cast<char*>(str->data())! See the unittest for why.
    return str->empty() ? nullptr : &*str->begin();
}

namespace io {
// From src/google/protobuf/io/zero_copy_stream_impl_lite.h
// mutable_string_data() and as_string_data() are workarounds to improve
// the performance of writing new data to an existing string.  Unfortunately
// the methods provided by the string class are suboptimal, and using memcpy()
// is mildly annoying because it requires its pointer args to be non-NULL even
// if we ask it to copy 0 bytes.  Furthermore, string_as_array() has the
// property that it always returns NULL if its arg is the empty string, exactly
// what we want to avoid if we're using it in conjunction with memcpy()!
// With C++11, the desired memcpy() boils down to memcpy(..., &(*s)[0], size),
// where s is a string*.  Without C++11, &(*s)[0] is not guaranteed to be safe,
// so we use string_as_array(), and live with the extra logic that tests whether
// *s is empty.

// Return a pointer to mutable characters underlying the given string.  The
// return value is valid until the next time the string is resized.  We
// trust the caller to treat the return value as an array of length s->size().
inline char* mutable_string_data(std::string* s) {
    // This should be simpler & faster than string_as_array() because the latter
    // is guaranteed to return NULL when *s is empty, so it has to check for that.
    return &(*s)[0];  // 这种实现不需要我们知道 std::string 的 memory layout.
}

// as_string_data(s) is equivalent to
//  ({ char* p = mutable_string_data(s); make_pair(p, p != NULL); })
// Sometimes it's faster: in some scenarios p cannot be NULL, and then the
// code can avoid that check.
inline std::pair<char*, bool> as_string_data(std::string* s) {
    char* p = mutable_string_data(s);
    return std::make_pair(p, true);
}
}  // namespace io

class MessageLite;

inline uint8_t* SerializeToArrayImpl(const MessageLite& msg, uint8_t* target, int size);

inline std::string alphabet();

class MessageLite {
  public:
    MessageLite() { _m = alphabet(); }

    bool SerializeToString(std::string* output) const {
        output->clear();
        return AppendToString(output);
    }

    bool AppendToString(std::string* output) const { return AppendPartialToString(output); }

    bool AppendPartialToString(std::string* output) const {
        size_t old_size = output->size();
        size_t byte_size = ByteSizeLong();
        if (byte_size > INT_MAX) {
            LOG(ERROR) << " exceeded maximum protobuf size of 2GB: " << byte_size;
            return false;
        }

        STLStringResizeUninitialized(output, old_size + byte_size);  // Resize the output to pre-allocate enough memory

        // reinterpret_cast 的操作数不是 std::string， 而是 string.data()
        uint8_t* start = reinterpret_cast<uint8_t*>(io::mutable_string_data(output) + old_size);
        SerializeToArrayImpl(*this, start, byte_size);
        return true;
    }

    size_t ByteSizeLong() const { return _m.size(); }

    std::string::const_iterator cbegin() const { return _m.cbegin(); }

    std::string::const_iterator cend() const { return _m.cend(); }

    const std::string& data() const { return _m; }

  private:
    std::string _m;
};

inline uint8_t* SerializeToArrayImpl(const MessageLite& msg, uint8_t* target, int size) {
    // std::memcpy or
    std::copy(msg.cbegin(), msg.cend(), target);
}

std::string alphabet() {
    std::vector<char> vec;
    vec.resize(26);
    std::iota(vec.begin(), vec.end(), 'a');
    return std::string(vec.begin(), vec.end());
}
}  // namespace pb

TEST(message_lite, SerializeToString) {
    pb::MessageLite message_lite;

    std::string out;
    message_lite.SerializeToString(&out);
    EXPECT_EQ(message_lite.data(), out);
}

//
/*
    // Parses a protocol buffer contained in a string. Returns true on success.
    // This function takes a string in the (non-human-readable) binary wire
    // format, matching the encoding output by MessageLite::SerializeToString().
    // If you'd like to convert a human-readable string into a protocol buffer
    // object, see google::protobuf::TextFormat::ParseFromString().
  ABSL_ATTRIBUTE_REINITIALIZES bool ParseFromString(absl::string_view data);

    // Like ParseFromString(), but accepts messages that are missing required fields.
  ABSL_ATTRIBUTE_REINITIALIZES bool ParsePartialFromString(absl::string_view data);
*/
TEST(message_lite, ParseFromString) {}