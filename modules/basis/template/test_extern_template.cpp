#include <gtest/gtest.h>
#include "extern_template.h"

namespace google {
namespace protobuf {
// build-time performance enhancements
TEST(template, extern_template_enhance_buildtime_performance) {
    {
        StringPiece str("true");
        EXPECT_TRUE(internal::MergeFromImpl<true>(str));
        EXPECT_FALSE(internal::MergeFromImpl<false>(str));
    }

    {
        StringPiece str("yes");
        EXPECT_TRUE(internal::MergeFromImpl<true>(str));
        EXPECT_FALSE(internal::MergeFromImpl<false>(str));
    }
}

TEST(template, extern_template_enhance_buildtime_performance2) {
    {
        StringPiece str("false");
        EXPECT_TRUE(internal::MergeFromImpl<false>(str));
        EXPECT_FALSE(internal::MergeFromImpl<true>(str));
    }

    {
        StringPiece str("no");
        EXPECT_TRUE(internal::MergeFromImpl<false>(str));
        EXPECT_FALSE(internal::MergeFromImpl<true>(str));
    }
}
}  // namespace protobuf
}  // namespace google

