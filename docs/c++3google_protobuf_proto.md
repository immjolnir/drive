# [Proto](https://protobuf.dev/programming-guides/proto3/)


## Scalar Value Types

A scalar message field can have one of the following types – the table shows the type specified in the `.proto` file, and the corresponding type in the automatically generated class:

Notable cases:
* .Proto's int32, sint32, sfixed32 map to C++'s int32.
  - For example: modules/google_protobuf/test_builtin_wire_format_lite.cpp

* .Proto's uint32, fixed32 map to C++'s uint32.

* .Proto's int32, int64:
  - Uses variable-length encoding.
  - Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint32/sint64 instead.

* .Proto's uint32, uint64:
  - Uses variable-length encoding.

* .Proto's sint32, sint64:
  - Uses variable-length encoding.
  - Signed int value. These more efficiently encode negative numbers than regular int32/int64 s for using ZigZag coding.

* .Proto's fixed32
  - Always four bytes.
  - More efficient than uint32 if values are often greater than 2^28.
  
* .Proto's fixed64
  - Always eight bytes.
  - More efficient than uint64 if values are often greater than 256.

## Default Values
When a message is parsed, if the encoded message does not contain a particular singular element, the corresponding field in the parsed object is set to the default value for that field. These defaults are type-specific:

- For strings, the default value is the empty string.
- For bytes, the default value is empty bytes.
- For bools, the default value is false.
- For numeric types, the default value is zero.
- For enums, the default value is the first defined enum value, which must be 0.
- For message fields, the field is not set. Its exact value is language-dependent. See the generated code guide for details.

- The default value for repeated fields is empty (generally an empty list in the appropriate language).

- [.Proto's type definition](https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/type.proto)