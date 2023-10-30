#include <gtest/gtest.h>

/**
 * src/google/protobuf/io/coded_stream.h
 * src/google/protobuf/io/coded_stream.cc
 * src/google/protobuf/io/coded_stream_unittest.cc
 */
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include <glog/logging.h>

#include <sstream>  // istringstream

// This declares an unsigned long long integer literal in a portable way.
// (The original macro is way too big and ruins my formatting.)
#undef ULL
#define ULL(x) PROTOBUF_ULONGLONG(x)

// From google/protobuf/port_def.inc
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

// From src/google/protobuf/stubs/macros.h
//
// The GOOGLE_ARRAYSIZE(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.
//
// GOOGLE_ARRAYSIZE catches a few type errors.  If you see a compiler error
//
//   "warning: division by zero in ..."
//
// when using GOOGLE_ARRAYSIZE, you are (wrongfully) giving it a pointer.
// You should only use GOOGLE_ARRAYSIZE on statically allocated arrays.
//
// The following comments are on the implementation details, and can
// be ignored by the users.
//
// ARRAYSIZE(arr) works by inspecting sizeof(arr) (the # of bytes in
// the array) and sizeof(*(arr)) (the # of bytes in one array
// element).  If the former is divisible by the latter, perhaps arr is
// indeed an array, in which case the division result is the # of
// elements in the array.  Otherwise, arr cannot possibly be an array,
// and we generate a compiler error to prevent the code from
// compiling.
//
// Since the size of bool is implementation-defined, we need to cast
// !(sizeof(a) & sizeof(*(a))) to size_t in order to ensure the final
// result has type size_t.
//
// This macro is not perfect as it wrongfully accepts certain
// pointers, namely where the pointer size is divisible by the pointee
// size.  Since all our code has to go through a 32-bit compiler,
// where a pointer is 4 bytes, this means all pointers to a type whose
// size is 3 or greater than 4 will be (righteously) rejected.
//
// Kudos to Jorg Brown for this simple and elegant implementation.

#undef GOOGLE_ARRAYSIZE
#define GOOGLE_ARRAYSIZE(a) ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

// ===================================================================
// Data-Driven Test Infrastructure

// TEST_1D and TEST_2D are macros I'd eventually like to see added to
// gTest.  These macros can be used to declare tests which should be
// run multiple times, once for each item in some input array.  TEST_1D
// tests all cases in a single input array.  TEST_2D tests all
// combinations of cases from two arrays.  The arrays must be statically
// defined such that the GOOGLE_ARRAYSIZE() macro works on them.  Example:
//
// int kCases[] = {1, 2, 3, 4}
// TEST_1D(MyFixture, MyTest, kCases) {
//   EXPECT_GT(kCases_case, 0);
// }
//
// This test iterates through the numbers 1, 2, 3, and 4 and tests that
// they are all grater than zero.  In case of failure, the exact case
// which failed will be printed.  The case type must be printable using
// ostream::operator<<.

// TODO(kenton):  gTest now supports "parameterized tests" which would be
//   a better way to accomplish this.  Rewrite when time permits.

#define TEST_1D(FIXTURE, NAME, CASES)                                                      \
    class FIXTURE##_##NAME##_DD : public FIXTURE {                                         \
      protected:                                                                           \
        template <typename CaseType>                                                       \
        void DoSingleCase(const CaseType& CASES##_case);                                   \
    };                                                                                     \
                                                                                           \
    TEST_F(FIXTURE##_##NAME##_DD, NAME) {                                                  \
        for (int i = 0; i < GOOGLE_ARRAYSIZE(CASES); i++) {                                \
            SCOPED_TRACE(testing::Message() << #CASES " case #" << i << ": " << CASES[i]); \
            DoSingleCase(CASES[i]);                                                        \
        }                                                                                  \
    }                                                                                      \
                                                                                           \
    template <typename CaseType>                                                           \
    void FIXTURE##_##NAME##_DD::DoSingleCase(const CaseType& CASES##_case)

#define TEST_2D(FIXTURE, NAME, CASES1, CASES2)                                                         \
    class FIXTURE##_##NAME##_DD : public FIXTURE {                                                     \
      protected:                                                                                       \
        template <typename CaseType1, typename CaseType2>                                              \
        void DoSingleCase(const CaseType1& CASES1##_case, const CaseType2& CASES2##_case);             \
    };                                                                                                 \
                                                                                                       \
    TEST_F(FIXTURE##_##NAME##_DD, NAME) {                                                              \
        for (int i = 0; i < GOOGLE_ARRAYSIZE(CASES1); i++) {                                           \
            for (int j = 0; j < GOOGLE_ARRAYSIZE(CASES2); j++) {                                       \
                SCOPED_TRACE(testing::Message() << #CASES1 " case #" << i << ": " << CASES1[i] << ", " \
                                                << #CASES2 " case #" << j << ": " << CASES2[j]);       \
                DoSingleCase(CASES1[i], CASES2[j]);                                                    \
            }                                                                                          \
        }                                                                                              \
    }                                                                                                  \
                                                                                                       \
    template <typename CaseType1, typename CaseType2>                                                  \
    void FIXTURE##_##NAME##_DD::DoSingleCase(const CaseType1& CASES1##_case, const CaseType2& CASES2##_case)

// 上面的这行的 DoSignleCase 把 实现留给了 test case's definition.

// SCOPED_TRACE
//  ::testing::ScopedTrace gtest_trace_143

TEST(ARRAY_SIZE, primary_types) {
    int b[3];
    EXPECT_EQ(3, GOOGLE_ARRAYSIZE(b));

    int i[3];
    EXPECT_EQ(3, GOOGLE_ARRAYSIZE(i));

    char c[3];
    EXPECT_EQ(3, GOOGLE_ARRAYSIZE(c));

    double d[3];
    EXPECT_EQ(3, GOOGLE_ARRAYSIZE(d));

    std::string str[3];
    EXPECT_EQ(3, GOOGLE_ARRAYSIZE(str));

    std::string* ptr[3];
    EXPECT_EQ(3, GOOGLE_ARRAYSIZE(ptr));
}

namespace google::protobuf
{

    class CodedStreamTest : public testing::Test {
      protected:
        // Buffer used during most of the tests. This assumes tests run sequentially.
        static constexpr int kBufferSize = 1024 * 64;
        static uint8 buffer_[kBufferSize];
    };

    uint8 CodedStreamTest::buffer_[CodedStreamTest::kBufferSize];

    // We test each operation over a variety of block sizes to insure that
    // we test cases where reads or writes cross buffer boundaries, cases
    // where they don't, and cases where there is so much buffer left that
    // we can use special optimized paths that don't worry about bounds
    // checks.
    const int kBlockSizes[] = {1, 2, 3, 5, 7, 13, 32, 1024};

    // Varint tests.

    struct VarintCase {
        uint8 bytes[10];  // Encoded bytes.
        int size;         // Encoded size, in bytes.
        uint64 value;     // Parsed value.
    };

    inline std::ostream& operator<<(std::ostream& os, const VarintCase& c) { return os << c.value; }

    VarintCase kVarintCases[] = {
      // 32-bit values
      {{0x00}, 1, 0},
      {{0x01}, 1, 1},
      {{0x7f}, 1, 127},
      {{0xa2, 0x74}, 2, (0x22 << 0) | (0x74 << 7)},  // 14882
      {{0xbe, 0xf7, 0x92, 0x84, 0x0b},
       5,  // 2961488830
       (0x3e << 0) | (0x77 << 7) | (0x12 << 14) | (0x04 << 21) | (ULL(0x0b) << 28)},

      // 64-bit
      {{0xbe, 0xf7, 0x92, 0x84, 0x1b},
       5,  // 7256456126
       (0x3e << 0) | (0x77 << 7) | (0x12 << 14) | (0x04 << 21) | (ULL(0x1b) << 28)},
      {{0x80, 0xe6, 0xeb, 0x9c, 0xc3, 0xc9, 0xa4, 0x49},
       8,  // 41256202580718336
       (0x00 << 0) | (0x66 << 7) | (0x6b << 14) | (0x1c << 21) | (ULL(0x43) << 28) | (ULL(0x49) << 35) |
         (ULL(0x24) << 42) | (ULL(0x49) << 49)},
      // 11964378330978735131
      {{0x9b, 0xa8, 0xf9, 0xc2, 0xbb, 0xd6, 0x80, 0x85, 0xa6, 0x01},
       10,
       (0x1b << 0) | (0x28 << 7) | (0x79 << 14) | (0x42 << 21) | (ULL(0x3b) << 28) | (ULL(0x56) << 35) |
         (ULL(0x00) << 42) | (ULL(0x05) << 49) | (ULL(0x26) << 56) | (ULL(0x01) << 63)},
    };

    TEST_2D(CodedStreamTest, ReadVarint32, kVarintCases, kBlockSizes) {
        memcpy(buffer_, kVarintCases_case.bytes, kVarintCases_case.size);
        io::ArrayInputStream input(buffer_, sizeof(buffer_), kBlockSizes_case);

        {
            io::CodedInputStream coded_input(&input);

            uint32 value;
            EXPECT_TRUE(coded_input.ReadVarint32(&value));
            EXPECT_EQ(static_cast<uint32>(kVarintCases_case.value), value);
        }

        EXPECT_EQ(kVarintCases_case.size, input.ByteCount());
    }

    TEST_2D(CodedStreamTest, ReadTag, kVarintCases, kBlockSizes) {
        memcpy(buffer_, kVarintCases_case.bytes, kVarintCases_case.size);
        io::ArrayInputStream input(buffer_, sizeof(buffer_), kBlockSizes_case);

        {
            io::CodedInputStream coded_input(&input);

            uint32 expected_value = static_cast<uint32>(kVarintCases_case.value);
            EXPECT_EQ(expected_value, coded_input.ReadTag());

            EXPECT_TRUE(coded_input.LastTagWas(expected_value));
            EXPECT_FALSE(coded_input.LastTagWas(expected_value + 1));
        }

        EXPECT_EQ(kVarintCases_case.size, input.ByteCount());
    }

    TEST_1D(CodedStreamTest, ExpectTag, kVarintCases) {
        // Leave one byte at the beginning of the buffer so we can read it
        // to force the first buffer to be loaded.
        buffer_[0] = '\0';
        memcpy(buffer_ + 1, kVarintCases_case.bytes, kVarintCases_case.size);
        io::ArrayInputStream input(buffer_, sizeof(buffer_));

        {
            io::CodedInputStream coded_input(&input);

            // Read one byte to force coded_input.Refill() to be called.  Otherwise,
            // ExpectTag() will return a false negative.
            uint8 dummy;
            coded_input.ReadRaw(&dummy, 1);
            EXPECT_EQ((uint)'\0', (uint)dummy);

            uint32 expected_value = static_cast<uint32>(kVarintCases_case.value);

            // ExpectTag() produces false negatives for large values.
            if (kVarintCases_case.size <= 2) {
                EXPECT_FALSE(coded_input.ExpectTag(expected_value + 1));
                EXPECT_TRUE(coded_input.ExpectTag(expected_value));
            } else {
                EXPECT_FALSE(coded_input.ExpectTag(expected_value));
            }
        }

        if (kVarintCases_case.size <= 2) {
            EXPECT_EQ(kVarintCases_case.size + 1, input.ByteCount());
        } else {
            EXPECT_EQ(1, input.ByteCount());
        }
    }

    TEST_2D(CodedStreamTest, WriteVarint32, kVarintCases, kBlockSizes) {
        if (kVarintCases_case.value > ULL(0x00000000FFFFFFFF)) {
            // Skip this test for the 64-bit values.
            return;
        }

        io::ArrayOutputStream output(buffer_, sizeof(buffer_), kBlockSizes_case);

        {
            io::CodedOutputStream coded_output(&output);

            coded_output.WriteVarint32(static_cast<uint32>(kVarintCases_case.value));
            EXPECT_FALSE(coded_output.HadError());

            EXPECT_EQ(kVarintCases_case.size, coded_output.ByteCount());
        }

        EXPECT_EQ(kVarintCases_case.size, output.ByteCount());
        EXPECT_EQ(0, memcmp(buffer_, kVarintCases_case.bytes, kVarintCases_case.size));
    }

    // -------------------------------------------------------------------
    // Varint failure test.

    struct VarintErrorCase {
        uint8 bytes[12];
        int size;
        bool can_parse;
    };

    inline std::ostream& operator<<(std::ostream& os, const VarintErrorCase& c) { return os << "size " << c.size; }

    const VarintErrorCase kVarintErrorCases[] = {
      // Control case.  (Insures that there isn't something else wrong that
      // makes parsing always fail.)
      {{0x00}, 1, true},

      // No input data.
      {{}, 0, false},

      // Input ends unexpectedly.
      {{0xf0, 0xab}, 2, false},

      // Input ends unexpectedly after 32 bits.
      {{0xf0, 0xab, 0xc9, 0x9a, 0xf8, 0xb2}, 6, false},

      // Longer than 10 bytes.
      {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01}, 11, false},
    };

    TEST_2D(CodedStreamTest, ReadVarint32Error, kVarintErrorCases, kBlockSizes) {
        memcpy(buffer_, kVarintErrorCases_case.bytes, kVarintErrorCases_case.size);
        io::ArrayInputStream input(buffer_, kVarintErrorCases_case.size, kBlockSizes_case);
        io::CodedInputStream coded_input(&input);

        uint32 value;
        EXPECT_EQ(kVarintErrorCases_case.can_parse, coded_input.ReadVarint32(&value));
    }

    // -------------------------------------------------------------------
    // VarintSize

    struct VarintSizeCase {
        uint64 value;
        int size;
    };

    inline std::ostream& operator<<(std::ostream& os, const VarintSizeCase& c) { return os << c.value; }

    VarintSizeCase kVarintSizeCases[] = {
      {0u, 1},
      {1u, 1},
      {127u, 1},
      {128u, 2},
      {758923u, 3},
      {4000000000u, 5},
      {ULL(41256202580718336), 8},
      {ULL(11964378330978735131), 10},
    };

    TEST_1D(CodedStreamTest, VarintSize32, kVarintSizeCases) {
        if (kVarintSizeCases_case.value > 0xffffffffu) {
            // Skip 64-bit values.
            return;
        }

        EXPECT_EQ(kVarintSizeCases_case.size,
                  io::CodedOutputStream::VarintSize32(static_cast<uint32>(kVarintSizeCases_case.value)));
    }

    TEST_1D(CodedStreamTest, VarintSize64, kVarintSizeCases) {
        EXPECT_EQ(kVarintSizeCases_case.size, io::CodedOutputStream::VarintSize64(kVarintSizeCases_case.value));
    }

    // -------------------------------------------------------------------
    // Fixed-size int tests

    struct Fixed32Case {
        uint8 bytes[sizeof(uint32)];  // Encoded bytes.
        uint32 value;                 // Parsed value.
    };

    struct Fixed64Case {
        uint8 bytes[sizeof(uint64)];  // Encoded bytes.
        uint64 value;                 // Parsed value.
    };

    inline std::ostream& operator<<(std::ostream& os, const Fixed32Case& c) {
        return os << "0x" << std::hex << c.value << std::dec;
    }

    inline std::ostream& operator<<(std::ostream& os, const Fixed64Case& c) {
        return os << "0x" << std::hex << c.value << std::dec;
    }

    Fixed32Case kFixed32Cases[] = {
      {{0xef, 0xcd, 0xab, 0x90}, 0x90abcdefu},
      {{0x12, 0x34, 0x56, 0x78}, 0x78563412u},
    };

    Fixed64Case kFixed64Cases[] = {
      {{0xef, 0xcd, 0xab, 0x90, 0x12, 0x34, 0x56, 0x78}, ULL(0x7856341290abcdef)},
      {{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}, ULL(0x8877665544332211)},
    };

    TEST_2D(CodedStreamTest, ReadLittleEndian32, kFixed32Cases, kBlockSizes) {
        memcpy(buffer_, kFixed32Cases_case.bytes, sizeof(kFixed32Cases_case.bytes));
        io::ArrayInputStream input(buffer_, sizeof(buffer_), kBlockSizes_case);

        {
            io::CodedInputStream coded_input(&input);

            uint32 value;
            EXPECT_TRUE(coded_input.ReadLittleEndian32(&value));
            EXPECT_EQ(kFixed32Cases_case.value, value);
        }

        EXPECT_EQ(sizeof(uint32), input.ByteCount());
    }

    TEST_2D(CodedStreamTest, WriteLittleEndian32, kFixed32Cases, kBlockSizes) {
        io::ArrayOutputStream output(buffer_, sizeof(buffer_), kBlockSizes_case);

        {
            io::CodedOutputStream coded_output(&output);

            coded_output.WriteLittleEndian32(kFixed32Cases_case.value);
            EXPECT_FALSE(coded_output.HadError());

            EXPECT_EQ(sizeof(uint32), coded_output.ByteCount());
        }

        EXPECT_EQ(sizeof(uint32), output.ByteCount());
        EXPECT_EQ(0, memcmp(buffer_, kFixed32Cases_case.bytes, sizeof(uint32)));
    }

    // -------------------------------------------------------------------
    // Raw reads and writes

    const char kRawBytes[] = "Some bytes which will be written and read raw.";

    TEST_1D(CodedStreamTest, ReadRaw, kBlockSizes) {
        memcpy(buffer_, kRawBytes, sizeof(kRawBytes));
        io::ArrayInputStream input(buffer_, sizeof(buffer_), kBlockSizes_case);
        char read_buffer[sizeof(kRawBytes)];

        {
            io::CodedInputStream coded_input(&input);

            EXPECT_TRUE(coded_input.ReadRaw(read_buffer, sizeof(kRawBytes)));
            EXPECT_EQ(0, memcmp(kRawBytes, read_buffer, sizeof(kRawBytes)));
        }

        EXPECT_EQ(sizeof(kRawBytes), input.ByteCount());
    }

    TEST_1D(CodedStreamTest, WriteRaw, kBlockSizes) {
        io::ArrayOutputStream output(buffer_, sizeof(buffer_), kBlockSizes_case);

        {
            io::CodedOutputStream coded_output(&output);

            coded_output.WriteRaw(kRawBytes, sizeof(kRawBytes));
            EXPECT_FALSE(coded_output.HadError());

            EXPECT_EQ(sizeof(kRawBytes), coded_output.ByteCount());
        }

        EXPECT_EQ(sizeof(kRawBytes), output.ByteCount());
        EXPECT_EQ(0, memcmp(buffer_, kRawBytes, sizeof(kRawBytes)));
    }

}  // namespace google::protobuf

using namespace google::protobuf;

TEST(coded_stream, integer) {
    std::string str;
    int expect = 1234;

    {  // Write
        io::StringOutputStream* raw_output = new io::StringOutputStream(&str);
        io::CodedOutputStream* coded_output = new io::CodedOutputStream(raw_output);
        coded_output->WriteVarint32(expect);
        delete coded_output;
        delete raw_output;
    }
    {  // Read
        std::istringstream istream(str);
        io::IstreamInputStream* raw_input = new io::IstreamInputStream(&istream);
        io::CodedInputStream* coded_input = new io::CodedInputStream(raw_input);
        unsigned int actual;
        coded_input->ReadVarint32(&actual);
        EXPECT_EQ(expect, actual);
        delete coded_input;
        delete raw_input;
    }
}

TEST(coded_stream, string) {
    std::string str;
    char expect[] = "Hello world!";

    {  // Write
        io::StringOutputStream* raw_output = new io::StringOutputStream(&str);
        io::CodedOutputStream* coded_output = new io::CodedOutputStream(raw_output);
        coded_output->WriteVarint32(strlen(expect));
        coded_output->WriteRaw(expect, strlen(expect));
        delete coded_output;
        delete raw_output;
    }
    {  // Read
        std::istringstream istream(str);
        io::IstreamInputStream* raw_input = new io::IstreamInputStream(&istream);
        io::CodedInputStream* coded_input = new io::CodedInputStream(raw_input);
        uint32 size;
        coded_input->ReadVarint32(&size);
        char actual[size + 1];
        coded_input->ReadRaw(actual, size);
        actual[size] = '\0';

        std::cout << "Decode string: " << actual << std::endl;

        EXPECT_EQ(std::string(expect), std::string(actual));
        delete coded_input;
        delete raw_input;
    }
}

TEST(coded_stream, VarintSize32PowersOfTwo) {
    int expected = 1;
    for (int i = 1; i < 32; i++) {
        if (i % 7 == 0) {
            expected += 1;
        }
        EXPECT_EQ(expected, io::CodedOutputStream::VarintSize32(static_cast<uint32>(0x1u << i)));
    }
}

TEST(coded_stream, VarintSize64PowersOfTwo) {
    int expected = 1;
    for (int i = 1; i < 64; i++) {
        if (i % 7 == 0) {
            expected += 1;
        }
        EXPECT_EQ(expected, io::CodedOutputStream::VarintSize64(static_cast<uint64>(0x1ull << i)));
    }
}

// -------------------------------------------------------------------
// GetDirectBufferPointer

TEST(coded_stream, GetDirectBufferPointerInput) {
    uint8 buffer_[10];

    io::ArrayInputStream input(buffer_, sizeof(buffer_), 8);
    io::CodedInputStream coded_input(&input);

    const void* ptr;
    int size;

    EXPECT_TRUE(coded_input.GetDirectBufferPointer(&ptr, &size));
    EXPECT_EQ(buffer_, ptr);
    EXPECT_EQ(8, size);

    // Peeking again should return the same pointer.
    EXPECT_TRUE(coded_input.GetDirectBufferPointer(&ptr, &size));
    EXPECT_EQ(buffer_, ptr);
    EXPECT_EQ(8, size);

    // Skip forward in the same buffer then peek again.
    EXPECT_TRUE(coded_input.Skip(3));
    EXPECT_TRUE(coded_input.GetDirectBufferPointer(&ptr, &size));
    EXPECT_EQ(buffer_ + 3, ptr);
    EXPECT_EQ(5, size);

    // Skip to end of buffer and peek -- should get next buffer.
    EXPECT_TRUE(coded_input.Skip(5));
    EXPECT_TRUE(coded_input.GetDirectBufferPointer(&ptr, &size));
    EXPECT_EQ(buffer_ + 8, ptr);
    EXPECT_EQ(8, size);
}

TEST(coded_stream, ExpectAtEnd) {
    uint8 buffer_[10];

    // Test ExpectAtEnd(), which is based on limits.
    io::ArrayInputStream input(buffer_, sizeof(buffer_));
    io::CodedInputStream coded_input(&input);

    EXPECT_FALSE(coded_input.ExpectAtEnd());

    io::CodedInputStream::Limit limit = coded_input.PushLimit(4);

    uint32 value;
    EXPECT_TRUE(coded_input.ReadLittleEndian32(&value));
    EXPECT_TRUE(coded_input.ExpectAtEnd());

    coded_input.PopLimit(limit);
    EXPECT_FALSE(coded_input.ExpectAtEnd());
}

TEST(coded_stream, OverflowLimit) {
    uint8 buffer_[10];
    // Check what happens when we push a limit large enough that its absolute
    // position is more than 2GB into the stream.
    io::ArrayInputStream input(buffer_, sizeof(buffer_));
    io::CodedInputStream coded_input(&input);
    ASSERT_TRUE(coded_input.Skip(128));

    io::CodedInputStream::Limit limit = coded_input.PushLimit(INT_MAX);
    // BytesUntilLimit() returns -1 to mean "no limit", which actually means
    // "the limit is INT_MAX relative to the beginning of the stream".
    EXPECT_EQ(-1, coded_input.BytesUntilLimit());
    coded_input.PopLimit(limit);
}

class ReallyBigInputStream : public io::ZeroCopyInputStream {
  public:
    ReallyBigInputStream() : backup_amount_(0), buffer_count_(0) {}

    ~ReallyBigInputStream() {}

    // implements ZeroCopyInputStream ----------------------------------
    bool Next(const void** data, int* size) override {
        // We only expect BackUp() to be called at the end.
        EXPECT_EQ(0, backup_amount_);

        switch (buffer_count_++) {
            case 0:
                *data = buffer_;
                *size = sizeof(buffer_);
                return true;
            case 1:
                // Return an enormously large buffer that, when combined with the 1k
                // returned already, should overflow the total_bytes_read_ counter in
                // CodedInputStream.  Note that we'll only read the first 1024 bytes
                // of this buffer so it's OK that we have it point at buffer_.
                *data = buffer_;
                *size = INT_MAX;
                return true;
            default:
                return false;
        }
    }

    void BackUp(int count) override { backup_amount_ = count; }

    bool Skip(int count) override {
        LOG(FATAL) << "Not implemented.";
        return false;
    }

    int64_t ByteCount() const override {
        LOG(FATAL) << "Not implemented.";
        return 0;
    }

    int backup_amount_;

  private:
    char buffer_[1024];
    int64 buffer_count_;
};

TEST(coded_stream, InputOver2G) {
    // CodedInputStream should gracefully handle input over 2G and call
    // input.BackUp() with the correct number of bytes on destruction.
    ReallyBigInputStream input;

    std::vector<std::string> errors;

    {
        // ScopedMemoryLog error_log;
        io::CodedInputStream coded_input(&input);
        std::string str;
        EXPECT_TRUE(coded_input.ReadString(&str, 512));
        EXPECT_TRUE(coded_input.ReadString(&str, 1024));
        // errors = error_log.GetMessages(ERROR);
    }

    EXPECT_EQ(INT_MAX - 512, input.backup_amount_);
    EXPECT_EQ(0, errors.size());
}

