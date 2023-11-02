# ZigZag-encoded varint on the wire

## Google's ZigZag Varint

- https://protobuf.dev/programming-guides/encoding/#types

- src/google/protobuf/wire_format_lite.h
```c++
// ZigZag Transform:  Encodes signed integers so that they can be
// effectively used with varint encoding.
//
// varint operates on unsigned integers, encoding smaller numbers into
// fewer bytes.  If you try to use it on a signed integer, it will treat
// this number as a very large unsigned integer, which means that even
// small signed numbers like -1 will take the maximum number of bytes
// (10) to encode.  ZigZagEncode() maps signed integers to unsigned
// in such a way that those with a small absolute value will have smaller
// encoded values, making them appropriate for encoding using varint.
//
//       int32 ->     uint32
// -------------------------
//           0 ->          0
//          -1 ->          1
//           1 ->          2
//          -2 ->          3
//         ... ->        ...
//  2147483647 -> 4294967294
// -2147483648 -> 4294967295
//
//        >> encode >>
//        << decode <<

inline uint32 WireFormatLite::ZigZagEncode32(int32 n) {
  // Note:  the right-shift must be arithmetic
  // Note:  left shift must be unsigned because of overflow
  return (static_cast<uint32>(n) << 1) ^ static_cast<uint32>(n >> 31);
}

inline int32 WireFormatLite::ZigZagDecode32(uint32 n) {
  // Note:  Using unsigned types prevent undefined behavior
  return static_cast<int32>((n >> 1) ^ (~(n & 1) + 1));
}

inline uint64 WireFormatLite::ZigZagEncode64(int64 n) {
  // Note:  the right-shift must be arithmetic
  // Note:  left shift must be unsigned because of overflow
  return (static_cast<uint64>(n) << 1) ^ static_cast<uint64>(n >> 63);
}

inline int64 WireFormatLite::ZigZagDecode64(uint64 n) {
  // Note:  Using unsigned types prevent undefined behavior
  return static_cast<int64>((n >> 1) ^ (~(n & 1) + 1));
}
```

More info see: 
- [Variable-length quantity](https://en.wikipedia.org/wiki/Variable-length_quantity) or docs/wiki_variable_length_quantity.md (local copy).
