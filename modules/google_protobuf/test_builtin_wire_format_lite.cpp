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
    }
    {
        std::string serialized;
        proto::unittest::Sint32Message msg;
        msg.set_data(i);
        msg.SerializeToString(&serialized);
        EXPECT_EQ(2, serialized.length());
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
        EXPECT_EQ(33, serialized.length());  // Why?
    }
    {
        std::string serialized;
        proto::unittest::MoreSInt32Message msg;
        msg.add_data(i);
        // Add two more numbers
        msg.add_data(i);
        msg.add_data(i);

        msg.SerializeToString(&serialized);
        EXPECT_EQ(6, serialized.length());  // Why?
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

}  // namespace
}  // namespace internal
}  // namespace protobuf
}  // namespace google