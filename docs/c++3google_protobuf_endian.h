# Endian

- src/google/protobuf/parse_context.h
```c++
template <int>
struct EndianHelper;

template <>
struct EndianHelper<1> {
  static uint8 Load(const void* p) { return *static_cast<const uint8*>(p); }
};

template <>
struct EndianHelper<2> {
  static uint16 Load(const void* p) {
    uint16 tmp;
    std::memcpy(&tmp, p, 2);
#ifndef PROTOBUF_LITTLE_ENDIAN
    tmp = bswap_16(tmp);
#endif
    return tmp;
  }
};

template <>
struct EndianHelper<4> {
  static uint32 Load(const void* p) {
    uint32 tmp;
    std::memcpy(&tmp, p, 4);
#ifndef PROTOBUF_LITTLE_ENDIAN
    tmp = bswap_32(tmp);
#endif
    return tmp;
  }
};

template <>
struct EndianHelper<8> {
  static uint64 Load(const void* p) {
    uint64 tmp;
    std::memcpy(&tmp, p, 8);
#ifndef PROTOBUF_LITTLE_ENDIAN
    tmp = bswap_64(tmp);
#endif
    return tmp;
  }
};
```