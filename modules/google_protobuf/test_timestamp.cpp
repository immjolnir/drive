#include "proto/common/timestamp.pb.h"

#include <gtest/gtest.h>

#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

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

TEST(Timestamp, message_to_json_string) {
    proto::common::Timestamp timestamp;
    timestamp.set_seconds(1);
    timestamp.set_nanos(2);

    std::string actual;  // binary format
    google::protobuf::util::JsonOptions json_options;
    json_options.add_whitespace = true;
    /* Status src/google/protobuf/stubs/status.h
     Converts from protobuf message to JSON. This is a simple wrapper of BinaryToJsonString().
     It will use the DescriptorPool of the passed-in message to resolve Any types.

    LIBPROTOBUF_EXPORT util::Status MessageToJsonString(const Message& message,
                                   string* output,
                                   const JsonOptions& options);
    */
    EXPECT_TRUE(google::protobuf::util::MessageToJsonString(timestamp, &actual, json_options).ok());
    EXPECT_EQ("{\n \"seconds\": \"1\",\n \"nanos\": 2\n}\n", actual);
}

TEST(Timestamp, json_string_to_message) {
    std::string text = R"(
        {
           "seconds": 1,
           "nanos": 2
        }
    )";
    proto::common::Timestamp timestamp;
    /*
    Converts from JSON to protobuf message. This is a simple wrapper of JsonStringToBinary().
    It will use the DescriptorPool of the passed-in message to resolve Any types.
    LIBPROTOBUF_EXPORT util::Status JsonStringToMessage(const string& input,
                                   Message* message,
                                   const JsonParseOptions& options);
    */
    EXPECT_TRUE(google::protobuf::util::JsonStringToMessage(text, &timestamp).ok());
    EXPECT_EQ(1, timestamp.seconds());
    EXPECT_EQ(2, timestamp.nanos());
}