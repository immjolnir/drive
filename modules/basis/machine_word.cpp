#include <gtest/gtest.h>

#include <limits.h>  // for CHAR_BIT

TEST(machine_word, word_length) {
    // "A word is the amount of data that a machine can process at one time."
    //  "The size of a processor’s general-purpose registers (GPRs) is equal to its word size."
    // "Additionally, the size of the C type long is equal to the word size, whereas the size of the int type is
    // sometimes less than that of the word size"
    // -- Linux Kernel Development, Ch 17 (3rd edition, pg 381)
    // https://stackoverflow.com/questions/29519068/how-to-determine-processor-word-length-in-c

    EXPECT_EQ(8, sizeof(long));
    EXPECT_EQ(4, sizeof(int));

    EXPECT_EQ(8, sizeof(void*));  // word size
}

TEST(machine_word, size) {
    // https://stackoverflow.com/questions/3200954/what-is-char-bit
    // CHAR_BIT is the number of bits in char. These days, almost all architectures use 8 bits per byte but it is not
    // the case always.
    // Some older machines used to have 7-bit byte.
    // Some DSPs have 10 or more bit-bytes.

    // It can be found in <limits.h>.
    EXPECT_EQ(8, CHAR_BIT);
}