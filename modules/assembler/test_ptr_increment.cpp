#include <gmock/gmock.h>
#include <gtest/gtest.h>

/**
 * The case found
 * https://github.com/protocolbuffers/protobuf/blob/3667102d91bba05afcb777aa685b1014f521e9bc/src/google/protobuf/io/coded_stream.cc#L416
 *
// Read a varint from the given buffer, write it to *value, and return a pair.
// The first part of the pair is true iff the read was successful.  The second
// part is buffer + (number of bytes read).  This function is always inlined,
// so returning a pair is costless.
PROTOBUF_ALWAYS_INLINE
::std::pair<bool, const uint8_t*> ReadVarint32FromArray(uint32_t first_byte, const uint8_t* buffer, uint32_t* value);

inline ::std::pair<bool, const uint8_t*> ReadVarint32FromArray(uint32_t first_byte, const uint8_t* buffer,
                                                               uint32_t* value) {
    // Fast path:  We have enough bytes left in the buffer to guarantee that
    // this read won't cross the end, so we can skip the checks.
    ABSL_DCHECK_EQ(*buffer, first_byte);
    ABSL_DCHECK_EQ(first_byte & 0x80, 0x80) << first_byte;
    const uint8_t* ptr = buffer;
    uint32_t b;
    uint32_t result = first_byte - 0x80;
    ++ptr;  // We just processed the first byte.  Move on to the second.
    b = *(ptr++);
    result += b << 7;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 7;
    b = *(ptr++);
    result += b << 14;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 14;
    b = *(ptr++);
    result += b << 21;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 21;
    b = *(ptr++);
    result += b << 28;
    if (!(b & 0x80)) goto done;
    // "result -= 0x80 << 28" is irrelevant.

    // If the input is larger than 32 bits, we still need to read it all
    // and discard the high-order bits.
    for (int i = 0; i < kMaxVarintBytes - kMaxVarint32Bytes; i++) {
        b = *(ptr++);
        if (!(b & 0x80)) goto done;
    }

    // We have overrun the maximum size of a varint (10 bytes).  Assume
    // the data is corrupt.
    return std::make_pair(false, ptr);

done:
    *value = result;
    return std::make_pair(true, ptr);
}
 */
TEST(assembler, self_increment_operators) {
    char buffer[5] = {'a', 'b', 'c', 'd', 'e'};
    const char* ptr = buffer;

    ++ptr;
    EXPECT_EQ('b', *ptr);

    char b = *(ptr++);  // 这里的括号，只是为了代码可读；并不会改变 pre increment opreator 的执行方式。
    EXPECT_EQ('b', b);
    EXPECT_EQ('c', *ptr);
}

TEST(assembler, post_increment_operator) {
    int arr[3] = {1, 2, 3};

    int* p = arr;
    *(p++) = 5;
    /*
    equals to the two statements as below:
        *p = 5;
        p++;
    */
    EXPECT_EQ(2, *p);
    EXPECT_THAT(arr, testing::ElementsAre(5, 2, 3));
}

TEST(assembler, post_increment_operator2) {
    int arr[3] = {1, 2, 3};

    int* p = arr;
    *p++ = 5;  // As the same as `*(p++) = 5`;
    EXPECT_EQ(2, *p);
    EXPECT_THAT(arr, testing::ElementsAre(5, 2, 3));
}

TEST(assembler, pre_increment_operator) {
    int arr[3] = {1, 2, 3};

    int* p = arr;
    *(++p) = 5;
    /*
    equals to the two statements as below:
        ++p;
        *p = 5;
    */
    EXPECT_EQ(5, *p);
    EXPECT_THAT(arr, testing::ElementsAre(1, 5, 3));
}

TEST(assembler, pre_increment_operator2) {
    int arr[3] = {1, 2, 3};

    int* p = arr;
    *++p = 5;
    /*
    equals to the two statements as below:
        ++p;
        *p = 5;
    */
    EXPECT_EQ(5, *p);
    EXPECT_THAT(arr, testing::ElementsAre(1, 5, 3));
}
