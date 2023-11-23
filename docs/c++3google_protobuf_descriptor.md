# Descripter

- src/google/protobuf/descriptor.proto

- https://github.com/protocolbuffers/protobuf/issues/7623

Steps to reproduce the behavior:

- create comments.proto:
```
syntax = "proto3";
package testing;
message MessageWithComments {
    // Leading field comment.
    string foo = 1;
}
```

- Write a FileDescriptorSet for it:
```
$ protoc --descriptor_set_out temp comments.proto
```

- Text-format it:
```
protoc < temp --decode=google.protobuf.FileDescriptorSet google/protobuf/descriptor.proto
```

- What did you expect to see

SourceCodeInfo with comments:
```
file {
  name: "comments.proto"
  package: "testing"
  message_type {
    name: "MessageWithComments"
    field {
      name: "foo"
      number: 1
      label: LABEL_OPTIONAL
      type: TYPE_STRING
      json_name: "foo"
    }
  }
  source_code_info {
    …
    location {
      path: 4
      path: 0
      path: 2
      path: 0
      span: 4
      span: 4
      span: 19
      leading_comments: " Leading field comment.\n"
    }
    …
  }
  syntax: "proto3"
}
```

- What did you see instead?
```
$ protoc < temp --decode=google.protobuf.FileDescriptorSet google/protobuf/descriptor.proto
file {
  name: "comments.proto"
  package: "testing"
  message_type {
    name: "MessageWithComments"
    field {
      name: "foo"
      number: 1
      label: LABEL_OPTIONAL
      type: TYPE_STRING
      json_name: "foo"
    }
  }
  syntax: "proto3"
}
```
Writing a protoc plugin, wanted to use `--descriptor_set_out` to generate data for unit tests.

- https://gist.github.com/johnllao/5ffbe24a021891e7d887

- https://clement-jean.github.io/packed_vs_unpacked_repeated_fields/

## src/google/protobuf/descriptor.h

- FieldDescriptor
```c++
// Describes a single field of a message.  To get the descriptor for a given
// field, first get the Descriptor for the message in which it is defined,
// then call Descriptor::FindFieldByName().  To get a FieldDescriptor for
// an extension, do one of the following:
// - Get the Descriptor or FileDescriptor for its containing scope, then
//   call Descriptor::FindExtensionByName() or
//   FileDescriptor::FindExtensionByName().
// - Given a DescriptorPool, call DescriptorPool::FindExtensionByNumber() or
//   DescriptorPool::FindExtensionByPrintableName().
// Use DescriptorPool to construct your own descriptors.
class PROTOBUF_EXPORT FieldDescriptor {
 public:
  typedef FieldDescriptorProto Proto;

  // Identifies a field type.  0 is reserved for errors.  The order is weird
  // for historical reasons.  Types 12 and up are new in proto2.
  enum Type {
    TYPE_DOUBLE = 1,    // double, exactly eight bytes on the wire.
    TYPE_FLOAT = 2,     // float, exactly four bytes on the wire.
    TYPE_INT64 = 3,     // int64, varint on the wire.  Negative numbers
                        // take 10 bytes.  Use TYPE_SINT64 if negative
                        // values are likely.
    TYPE_UINT64 = 4,    // uint64, varint on the wire.
    TYPE_INT32 = 5,     // int32, varint on the wire.  Negative numbers
                        // take 10 bytes.  Use TYPE_SINT32 if negative
                        // values are likely.
    TYPE_FIXED64 = 6,   // uint64, exactly eight bytes on the wire.
    TYPE_FIXED32 = 7,   // uint32, exactly four bytes on the wire.
    TYPE_BOOL = 8,      // bool, varint on the wire.
    TYPE_STRING = 9,    // UTF-8 text.
    TYPE_GROUP = 10,    // Tag-delimited message.  Deprecated.
    TYPE_MESSAGE = 11,  // Length-delimited message.

    TYPE_BYTES = 12,     // Arbitrary byte array.
    TYPE_UINT32 = 13,    // uint32, varint on the wire
    TYPE_ENUM = 14,      // Enum, varint on the wire
    TYPE_SFIXED32 = 15,  // int32, exactly four bytes on the wire
    TYPE_SFIXED64 = 16,  // int64, exactly eight bytes on the wire
    TYPE_SINT32 = 17,    // int32, ZigZag-encoded varint on the wire
    TYPE_SINT64 = 18,    // int64, ZigZag-encoded varint on the wire

    MAX_TYPE = 18,  // Constant useful for defining lookup tables
                    // indexed by Type.
  };

  // Specifies the C++ data type used to represent the field.  There is a
  // fixed mapping from Type to CppType where each Type maps to exactly one
  // CppType.  0 is reserved for errors.
  enum CppType {
    CPPTYPE_INT32 = 1,     // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
    CPPTYPE_INT64 = 2,     // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
    CPPTYPE_UINT32 = 3,    // TYPE_UINT32, TYPE_FIXED32
    CPPTYPE_UINT64 = 4,    // TYPE_UINT64, TYPE_FIXED64
    CPPTYPE_DOUBLE = 5,    // TYPE_DOUBLE
    CPPTYPE_FLOAT = 6,     // TYPE_FLOAT
    CPPTYPE_BOOL = 7,      // TYPE_BOOL
    CPPTYPE_ENUM = 8,      // TYPE_ENUM
    CPPTYPE_STRING = 9,    // TYPE_STRING, TYPE_BYTES
    CPPTYPE_MESSAGE = 10,  // TYPE_MESSAGE, TYPE_GROUP

    MAX_CPPTYPE = 10,  // Constant useful for defining lookup tables
                       // indexed by CppType.
  };
  ...
};
```