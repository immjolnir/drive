/*
src/google/protobuf/wire_format_unittest.cc
*/

#include <gtest/gtest.h>

#include <google/protobuf/wire_format.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/logging.h>
#include <google/protobuf/wire_format_lite.h>

#include "proto/builtin/unittest.pb.h"

#include <fstream>

// Source from  <google/protobuf/port_def.inc>  // PROTOBUF_ULONGLONG
#ifdef _MSC_VER
#define PROTOBUF_LONGLONG(x) x##I64
#define PROTOBUF_ULONGLONG(x) x##UI64
#define PROTOBUF_LL_FORMAT "I64"  // As in printf("%I64d", ...)
#else
// By long long, we actually mean int64.
#define PROTOBUF_LONGLONG(x) x##LL
#define PROTOBUF_ULONGLONG(x) x##ULL
// Used to format real long long integers.
#define PROTOBUF_LL_FORMAT "ll"  // As in "%lld". Note that "q" is poor form also.
#endif

namespace google {
namespace protobuf {
namespace internal {
namespace {
TEST(WireFormatTest, ZigZag) {  // x
// avoid line-wrapping
#define LL(x) PROTOBUF_LONGLONG(x)
#define ULL(x) PROTOBUF_ULONGLONG(x)
/*
inline uint32 WireFormatLite::ZigZagEncode32(int32 n) {
  // Note:  the right-shift must be arithmetic
  // Note:  left shift must be unsigned because of overflow
  return (static_cast<uint32>(n) << 1) ^ static_cast<uint32>(n >> 31);
}

inline int32 WireFormatLite::ZigZagDecode32(uint32 n) {
  // Note:  Using unsigned types prevent undefined behavior
  return static_cast<int32>((n >> 1) ^ (~(n & 1) + 1));
}
*/
#define ZigZagEncode32(x) WireFormatLite::ZigZagEncode32(x)
#define ZigZagDecode32(x) WireFormatLite::ZigZagDecode32(x)
#define ZigZagEncode64(x) WireFormatLite::ZigZagEncode64(x)
#define ZigZagDecode64(x) WireFormatLite::ZigZagDecode64(x)

    EXPECT_EQ(0u, ZigZagEncode32(0));
    EXPECT_EQ(1u, ZigZagEncode32(-1));
    EXPECT_EQ(2u, ZigZagEncode32(1));
    EXPECT_EQ(3u, ZigZagEncode32(-2));
    EXPECT_EQ(0x7FFFFFFEu, ZigZagEncode32(0x3FFFFFFF));
    EXPECT_EQ(0x7FFFFFFFu, ZigZagEncode32(0xC0000000));
    EXPECT_EQ(0xFFFFFFFEu, ZigZagEncode32(0x7FFFFFFF));
    EXPECT_EQ(0xFFFFFFFFu, ZigZagEncode32(0x80000000));

    EXPECT_EQ(0, ZigZagDecode32(0u));
    EXPECT_EQ(-1, ZigZagDecode32(1u));
    EXPECT_EQ(1, ZigZagDecode32(2u));
    EXPECT_EQ(-2, ZigZagDecode32(3u));
    EXPECT_EQ(0x3FFFFFFF, ZigZagDecode32(0x7FFFFFFEu));
    EXPECT_EQ(0xC0000000, ZigZagDecode32(0x7FFFFFFFu));
    EXPECT_EQ(0x7FFFFFFF, ZigZagDecode32(0xFFFFFFFEu));
    EXPECT_EQ(0x80000000, ZigZagDecode32(0xFFFFFFFFu));

    EXPECT_EQ(0u, ZigZagEncode64(0));
    EXPECT_EQ(1u, ZigZagEncode64(-1));
    EXPECT_EQ(2u, ZigZagEncode64(1));
    EXPECT_EQ(3u, ZigZagEncode64(-2));
    EXPECT_EQ(ULL(0x000000007FFFFFFE), ZigZagEncode64(LL(0x000000003FFFFFFF)));
    EXPECT_EQ(ULL(0x000000007FFFFFFF), ZigZagEncode64(LL(0xFFFFFFFFC0000000)));
    EXPECT_EQ(ULL(0x00000000FFFFFFFE), ZigZagEncode64(LL(0x000000007FFFFFFF)));
    EXPECT_EQ(ULL(0x00000000FFFFFFFF), ZigZagEncode64(LL(0xFFFFFFFF80000000)));
    EXPECT_EQ(ULL(0xFFFFFFFFFFFFFFFE), ZigZagEncode64(LL(0x7FFFFFFFFFFFFFFF)));
    EXPECT_EQ(ULL(0xFFFFFFFFFFFFFFFF), ZigZagEncode64(LL(0x8000000000000000)));

    EXPECT_EQ(0, ZigZagDecode64(0u));
    EXPECT_EQ(-1, ZigZagDecode64(1u));
    EXPECT_EQ(1, ZigZagDecode64(2u));
    EXPECT_EQ(-2, ZigZagDecode64(3u));
    EXPECT_EQ(LL(0x000000003FFFFFFF), ZigZagDecode64(ULL(0x000000007FFFFFFE)));
    EXPECT_EQ(LL(0xFFFFFFFFC0000000), ZigZagDecode64(ULL(0x000000007FFFFFFF)));
    EXPECT_EQ(LL(0x000000007FFFFFFF), ZigZagDecode64(ULL(0x00000000FFFFFFFE)));
    EXPECT_EQ(LL(0xFFFFFFFF80000000), ZigZagDecode64(ULL(0x00000000FFFFFFFF)));
    EXPECT_EQ(LL(0x7FFFFFFFFFFFFFFF), ZigZagDecode64(ULL(0xFFFFFFFFFFFFFFFE)));
    EXPECT_EQ(LL(0x8000000000000000), ZigZagDecode64(ULL(0xFFFFFFFFFFFFFFFF)));

    // Some easier-to-verify round-trip tests.  The inputs (other than 0, 1, -1)
    // were chosen semi-randomly via keyboard bashing.
    EXPECT_EQ(0, ZigZagDecode32(ZigZagEncode32(0)));
    EXPECT_EQ(1, ZigZagDecode32(ZigZagEncode32(1)));
    EXPECT_EQ(-1, ZigZagDecode32(ZigZagEncode32(-1)));  // x
    EXPECT_EQ(14927, ZigZagDecode32(ZigZagEncode32(14927)));
    EXPECT_EQ(-3612, ZigZagDecode32(ZigZagEncode32(-3612)));

    EXPECT_EQ(0, ZigZagDecode64(ZigZagEncode64(0)));
    EXPECT_EQ(1, ZigZagDecode64(ZigZagEncode64(1)));
    EXPECT_EQ(-1, ZigZagDecode64(ZigZagEncode64(-1)));
    EXPECT_EQ(14927, ZigZagDecode64(ZigZagEncode64(14927)));
    EXPECT_EQ(-3612, ZigZagDecode64(ZigZagEncode64(-3612)));

    EXPECT_EQ(LL(856912304801416), ZigZagDecode64(ZigZagEncode64(LL(856912304801416))));
    EXPECT_EQ(LL(-75123905439571256), ZigZagDecode64(ZigZagEncode64(LL(-75123905439571256))));
}

/*
TEST(WireFormatTest, RepeatedScalarsDifferentTagSizes) {
    // At one point checks would trigger when parsing repeated fixed scalar
    // fields.
    protobuf_unittest::TestRepeatedScalarDifferentTagSizes msg1, msg2;  // x
    for (int i = 0; i < 100; ++i) {
        msg1.add_repeated_fixed32(i);
        msg1.add_repeated_int32(i);
        msg1.add_repeated_fixed64(i);
        msg1.add_repeated_int64(i);
        msg1.add_repeated_float(i);
        msg1.add_repeated_uint64(i);
    }

    // Make sure that we have a variety of tag sizes.
    const Descriptor* desc = msg1.GetDescriptor();
    const FieldDescriptor* field;
    field = desc->FindFieldByName("repeated_fixed32");
    ASSERT_TRUE(field != NULL);
    ASSERT_EQ(1, WireFormat::TagSize(field->number(), field->type()));
    field = desc->FindFieldByName("repeated_int32");
    ASSERT_TRUE(field != NULL);
    ASSERT_EQ(1, WireFormat::TagSize(field->number(), field->type()));
    field = desc->FindFieldByName("repeated_fixed64");
    ASSERT_TRUE(field != NULL);
    ASSERT_EQ(2, WireFormat::TagSize(field->number(), field->type()));
    field = desc->FindFieldByName("repeated_int64");
    ASSERT_TRUE(field != NULL);
    ASSERT_EQ(2, WireFormat::TagSize(field->number(), field->type()));
    field = desc->FindFieldByName("repeated_float");
    ASSERT_TRUE(field != NULL);
    ASSERT_EQ(3, WireFormat::TagSize(field->number(), field->type()));
    field = desc->FindFieldByName("repeated_uint64");
    ASSERT_TRUE(field != NULL);
    ASSERT_EQ(3, WireFormat::TagSize(field->number(), field->type()));

    EXPECT_TRUE(msg2.ParseFromString(msg1.SerializeAsString()));  // x
    EXPECT_EQ(msg1.DebugString(), msg2.DebugString());
}
*/
TEST(WireFormatTest, CompatibleTypes) {
    const int64 data = 0x100000000LL;
    proto::unittest::Int64Message msg1;
    msg1.set_data(data);
    std::string serialized;
    msg1.SerializeToString(&serialized);

    // Test int64 is compatible with bool
    proto::unittest::BoolMessage msg2;
    ASSERT_TRUE(msg2.ParseFromString(serialized));
    ASSERT_EQ(static_cast<bool>(data), msg2.data());

    // Test int64 is compatible with uint64
    proto::unittest::Uint64Message msg3;
    ASSERT_TRUE(msg3.ParseFromString(serialized));
    ASSERT_EQ(static_cast<uint64>(data), msg3.data());

    // Test int64 is compatible with int32
    proto::unittest::Int32Message msg4;
    ASSERT_TRUE(msg4.ParseFromString(serialized));
    ASSERT_EQ(static_cast<int32>(data), msg4.data());

    // Test int64 is compatible with uint32
    proto::unittest::Uint32Message msg5;
    ASSERT_TRUE(msg5.ParseFromString(serialized));
    ASSERT_EQ(static_cast<uint32>(data), msg5.data());
}

TEST(RepeatedVarint, EncodedSizeComparision_negative_data) {
    int i = -1;
    // int32 vs sint32
    {
        std::string serialized;
        proto::unittest::Int32Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(11, serialized.length());
        // hexedit
        // 08 FF FF FF  FF FF FF FF  FF FF 01
        // https://en.cppreference.com/w/cpp/io/ios_base/openmode
        // std::ofstream out("int32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }
    {
        std::string serialized;
        proto::unittest::Sint32Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(2, serialized.length());
        // hexedit
        // 08 01
        // std::ofstream out("sint32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }

    // int64 vs sint64
    {
        std::string serialized;
        proto::unittest::Int64Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(11, serialized.length());  // Why it has the same size as the Int32Message?
    }
    {
        std::string serialized;
        proto::unittest::Sint64Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(2, serialized.length());  // Yes, it should have the same size as Sint32Message!
    }
}

TEST(RepeatedVarint, EncodedSizeComparision_repeated_negative_data) {
    int i = -1;
    // int32 vs sint32
    {
        std::string serialized;
        proto::unittest::MoreInt32Message msg;
        msg.add_data(i);
        // Add two more numbers comparing the above case
        msg.add_data(i);
        msg.add_data(i);

        msg.SerializeToString(&serialized);
        EXPECT_EQ(33, serialized.length());  // Why? 11 * 3
        // hexed it
        // 08 FF FF FF  FF FF FF FF  FF FF 01
        // 08 FF FF FF  FF FF FF FF  FF FF 01
        // 08 FF FF FF  FF FF FF FF  FF FF 01
        // std::ofstream out("repeated_int32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }
    {
        std::string serialized;
        proto::unittest::MoreSInt32Message msg;
        msg.add_data(i);
        // Add two more numbers
        msg.add_data(i);
        msg.add_data(i);

        msg.SerializeToString(&serialized);
        EXPECT_EQ(6, serialized.length());  // Why? 2 * 3
        // hexedit
        // 08 01 08 01  08 01 // Why the tag(08) is repeated so many times?
        // std::ofstream out("repeated_sint32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }
}

TEST(RepeatedVarint, EncodedSizeComparision_positive_data) {
    int i = 1;
    // int32 vs uint32 vs fixed32 vs sint32
    {
        std::string serialized;
        proto::unittest::Int32Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(2, serialized.length());
        // hexedit
        // 08 01
        // https://en.cppreference.com/w/cpp/io/ios_base/openmode
        // std::ofstream out("int32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }
    {
        std::string serialized;
        proto::unittest::Uint32Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(2, serialized.length());
        // hexedit
        // 08 01
        // https://en.cppreference.com/w/cpp/io/ios_base/openmode
        // std::ofstream out("uint32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }
    {
        std::string serialized;
        proto::unittest::Fint32Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(5, serialized.length());
        // hexedit
        // 0D 01 00 00  00
        // https://en.cppreference.com/w/cpp/io/ios_base/openmode
        // std::ofstream out("fint32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }
    {
        std::string serialized;
        proto::unittest::Sint32Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(2, serialized.length());
        // hexedit
        // 08 02, but why?
        // std::ofstream out("sint32.bin", std::ios::out | std::ios::trunc | std::ios::binary);
        // out << serialized;
    }
}

TEST(RepeatedVarint, DecodedSizeComparision_positive_data) {
    const uint8_t buffer[2] = {0x08, 0x02};
    /*
    error: 'reinterpret_cast' from type 'const uint8_t*' {aka 'const unsigned char*'} to type 'char*' casts away
  qualifiers
  308 |     const std::string serialized = reinterpret_cast<char*>(buffer);
    */
    // ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffe02e8e062 at pc 0x7fadd2fc1aa7 bp 0x7ffe02e8dcc0
    // sp 0x7ffe02e8d468
    // https://iq.opengenus.org/convert-array-of-uint8t-to-string-in-cpp/
    // const std::string serialized = reinterpret_cast<char*>(const_cast<uint8_t*>(buffer));
    // std::string serialized;
    // serialized.resize(sizeof(buffer));                                         // Pre-Allocation the space for memcpy
    // std::memcpy(const_cast<uint8_t*>(buffer), serialized[0], sizeof(buffer));  // Won't change the str's length
    // EXPECT_EQ(2, serialized.length());

    // Convert uint8_t array to string
    // https://stackoverflow.com/questions/55260989/convert-uint8-t-array-to-string
    std::string serialized(reinterpret_cast<char const*>(buffer), sizeof(buffer));
    EXPECT_EQ(2, serialized.length());
    {
        proto::unittest::Sint32Message msg;
        ASSERT_TRUE(msg.ParseFromString(serialized));
        ASSERT_EQ(1, msg.data());
    }
    {  // Encode VS Decode
        EXPECT_EQ(2u, ZigZagEncode32(1));
        EXPECT_EQ(1, ZigZagDecode32(2u));
    }
    {
        io::ArrayInputStream input(buffer, 11, 1);
        io::CodedInputStream coded_input(&input);

        uint32 tag = 8;
        EXPECT_EQ(tag, coded_input.ReadTag());

        uint32 value;
        EXPECT_EQ(true, coded_input.ReadVarint32(&value));
        EXPECT_EQ(2u, value);
        EXPECT_EQ(1, ZigZagDecode32(2u));
        // So, for sint32, the decoding flow is ReadVarint32 -> uint32 -> ZigZagDecode32 -> int32.
    }
}

TEST(RepeatedVarint, Int32) {
    RepeatedField<int32> v;

    // Insert -2^n, 2^n and 2^n-1.
    for (int n = 0; n < 10; n++) {
        v.Add(-(1 << n));
        v.Add(1 << n);
        v.Add((1 << n) - 1);
    }

    // Check consistency with the scalar Int32Size.
    size_t expected = 0;
    for (int i = 0; i < v.size(); i++) {
        expected += WireFormatLite::Int32Size(v[i]);
    }

    EXPECT_EQ(expected, WireFormatLite::Int32Size(v));
}

TEST(RepeatedVarint, Int64) {
    RepeatedField<int64> v;

    // Insert -2^n, 2^n and 2^n-1.
    for (int n = 0; n < 10; n++) {
        v.Add(-(1 << n));
        v.Add(1 << n);
        v.Add((1 << n) - 1);
    }

    // Check consistency with the scalar Int64Size.
    size_t expected = 0;
    for (int i = 0; i < v.size(); i++) {
        expected += WireFormatLite::Int64Size(v[i]);
    }

    EXPECT_EQ(expected, WireFormatLite::Int64Size(v));
}

TEST(RepeatedVarint, SInt32) {
    RepeatedField<int32> v;

    // Insert -2^n, 2^n and 2^n-1.
    for (int n = 0; n < 10; n++) {
        v.Add(-(1 << n));
        v.Add(1 << n);
        v.Add((1 << n) - 1);
    }

    // Check consistency with the scalar SInt32Size.
    size_t expected = 0;
    for (int i = 0; i < v.size(); i++) {
        expected += WireFormatLite::SInt32Size(v[i]);
    }

    EXPECT_EQ(expected, WireFormatLite::SInt32Size(v));
}

TEST(RepeatedVarint, SInt64) {
    RepeatedField<int64> v;

    // Insert -2^n, 2^n and 2^n-1.
    for (int n = 0; n < 10; n++) {
        v.Add(-(1 << n));
        v.Add(1 << n);
        v.Add((1 << n) - 1);
    }

    // Check consistency with the scalar SInt64Size.
    size_t expected = 0;
    for (int i = 0; i < v.size(); i++) {
        expected += WireFormatLite::SInt64Size(v[i]);
    }

    EXPECT_EQ(expected, WireFormatLite::SInt64Size(v));
}

TEST(RepeatedVarint, UInt32) {
    RepeatedField<uint32> v;

    // Insert 2^n and 2^n-1.
    for (int n = 0; n < 10; n++) {
        v.Add(1 << n);
        v.Add((1 << n) - 1);
    }

    // Check consistency with the scalar UInt32Size.
    size_t expected = 0;
    for (int i = 0; i < v.size(); i++) {
        expected += WireFormatLite::UInt32Size(v[i]);
    }

    EXPECT_EQ(expected, WireFormatLite::UInt32Size(v));
}

TEST(RepeatedVarint, UInt64) {
    RepeatedField<uint64> v;

    // Insert 2^n and 2^n-1.
    for (int n = 0; n < 10; n++) {
        v.Add(1 << n);
        v.Add((1 << n) - 1);
    }

    // Check consistency with the scalar UInt64Size.
    size_t expected = 0;
    for (int i = 0; i < v.size(); i++) {
        expected += WireFormatLite::UInt64Size(v[i]);
    }

    EXPECT_EQ(expected, WireFormatLite::UInt64Size(v));
}

TEST(RepeatedVarint, Enum) {
    RepeatedField<int> v;

    // Insert 2^n and 2^n-1.
    for (int n = 0; n < 10; n++) {
        v.Add(1 << n);
        v.Add((1 << n) - 1);
    }

    // Check consistency with the scalar EnumSize.
    size_t expected = 0;
    for (int i = 0; i < v.size(); i++) {
        expected += WireFormatLite::EnumSize(v[i]);
    }

    EXPECT_EQ(expected, WireFormatLite::EnumSize(v));
}

/*
TEST(RepeatedVarint, ReadPrimitive_int32_vs_sint32) {
    // Generated by:
    //     std::string serialized;
    //     proto::unittest::Sint32Message msg;
    //     msg.set_data(i);
    //     msg.SerializeToString(&serialized);
    const uint8_t buffer[2] = {0x08, 0x02};
    io::ArrayInputStream input(buffer, 11, 1);
    io::CodedInputStream coded_input(&input);
}
*/

}  // namespace
}  // namespace internal
}  // namespace protobuf
}  // namespace google
