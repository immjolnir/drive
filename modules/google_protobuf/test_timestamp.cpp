#include "proto/common/timestamp.pb.h"

#include <gtest/gtest.h>

TEST(Timestamp, write_and_read) {
    proto::common::Timestamp timestamp;
    timestamp.set_seconds(1);
    timestamp.set_nanos(2);
    EXPECT_EQ(1, timestamp.seconds());
    EXPECT_EQ(2, timestamp.nanos());
}
