
## APIs

- [TextFormat](https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/text_format.h)

    - ParseFromString: convert a human-readable string into a protocol buffer object.
      - `static bool ParseFromString(absl::string_view input, Message* output);`

    - PrintToString: convert a protobuf buffer object to a human-readable string.
      - `static bool PrintToString(const Message& message, std::string* output);`
      - Note: output will be cleared prior to printing, and will be left empty even if printing fails. Returns false if printing fails.

- [MessageLite](https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/message_lite.h)
  - ParseFromString

```
  // Parses a protocol buffer contained in a string. Returns true on success.
  // This function takes a string in the (non-human-readable) binary wire
  // format, matching the encoding output by MessageLite::SerializeToString().
  // If you'd like to convert a human-readable string into a protocol buffer
  // object, see google::protobuf::TextFormat::ParseFromString().
  PROTOBUF_ATTRIBUTE_REINITIALIZES bool ParseFromString(absl::string_view data);
```

  - ParsePartialFromString
```
  // Like ParseFromString(), but accepts messages that are missing required fields.
  PROTOBUF_ATTRIBUTE_REINITIALIZES bool ParsePartialFromString(absl::string_view data);
```

  - SerializeToString
```
  // Serialize the message and store it in the given string.  All required
  // fields must be set.
  bool SerializeToString(std::string* output) const;
```
