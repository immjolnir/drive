#include "arrow_buffer.h"
#include "arrow_status.h"

#include <memory>
#include <string>

#include <gtest/gtest.h>

namespace arrow
{
    TEST(buffer, zero_copy) {
        // slicing an Array is more complicated because of the null bitmap.
        std::string data_str = "some data to slice";

        auto data = reinterpret_cast<const uint8_t*>(data_str.c_str());

        auto buf = std::make_shared<Buffer>(data, data_str.size());

        std::shared_ptr<Buffer> out = buf->slice(5, 4);
        Buffer expected(data + 5, 4);

        EXPECT_TRUE(out->Equals(expected));
        EXPECT_EQ(1, buf.use_count());
    }

    TEST(status, status_ok) {
        Status s;
        EXPECT_TRUE(s.ok());
    }

    TEST(status, construct) {
        Status s;
        s.FromArgs(StatusCode::OutOfMemory, "abc", "123");

        EXPECT_FALSE(s.ok());
        EXPECT_TRUE(s.IsOutOfMemory());
        EXPECT_EQ("", s.message());
    }
}  // namespace arrow