#include "proto/common/timestamp.pb.h"

#include <gtest/gtest.h>

#include <google/protobuf/text_format.h>

TEST(Timestamp, write_and_read) {
    proto::common::Timestamp timestamp;
    timestamp.set_seconds(1);
    timestamp.set_nanos(2);
    EXPECT_EQ(1, timestamp.seconds());
    EXPECT_EQ(2, timestamp.nanos());
}

TEST(Timestamp, TextFromat_to_string) {
    proto::common::Timestamp timestamp;
    timestamp.set_seconds(1);
    timestamp.set_nanos(2);

    std::string text;
    EXPECT_TRUE(google::protobuf::TextFormat::PrintToString(timestamp, &text));

    EXPECT_EQ("seconds: 1\nnanos: 2\n", text);
}

/*
  // Parses a protocol buffer contained in a string. Returns true on success.
  // This function takes a string in the (non-human-readable) binary wire
  // format, matching the encoding output by MessageLite::SerializeToString().
  // If you'd like to convert a human-readable string into a protocol buffer
  // object, see google::protobuf::TextFormat::ParseFromString().
*/

TEST(Timestamp, TextFromat_from_string) {
    proto::common::Timestamp timestamp;
    /*
    https://en.cppreference.com/w/cpp/language/string_literal

    prefix (optional) R"d-char-sequence (optional)(r-char-sequence (optional))d-char-sequence (optional)"

    Raw string literal. Used to avoid escaping of any character.
    Anything between the delimiters becomes part of the string. prefix, if present, has the same meaning as described
    above.
    */
    std::string text = R"(
        seconds: 1
        nanos: 2
    )";
    // https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/text_format.h
    EXPECT_TRUE(google::protobuf::TextFormat::ParseFromString(text, &timestamp));
    EXPECT_EQ(1, timestamp.seconds());
    EXPECT_EQ(2, timestamp.nanos());
}

TEST(Timestamp, binary_write) {
    proto::common::Timestamp timestamp;
    timestamp.set_seconds(1);
    timestamp.set_nanos(2);

    std::string actual;  // binary format
    EXPECT_TRUE(timestamp.SerializeToString(&actual));
    EXPECT_EQ("\b\x1\x10\x2", actual);

    {
        proto::common::Timestamp timestamp;
        EXPECT_TRUE(timestamp.ParseFromString(actual));
        EXPECT_EQ(1, timestamp.seconds());
        EXPECT_EQ(2, timestamp.nanos());
    }
}