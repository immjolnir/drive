#include "config.h"

#include <gtest/gtest.h>

#include <string>

using namespace std;
using namespace vot;

TEST(vot, load_vot_data) {
    std::string config_file = "config.yaml";
    Config config;
    EXPECT_TRUE(config.load(config_file));

    auto ret = config.get("Bolt");
    EXPECT_TRUE(ret.second);
    auto sequence = ret.first;
    EXPECT_EQ("Bolt", sequence.name);
    EXPECT_EQ("TB-50", sequence.location);
    // EXPECT_EQ("/work/data/Visual-Tracker-Benchmark/TB-50/", sequence.location);
    EXPECT_EQ("img", sequence.image_folder);
    EXPECT_EQ("{:04}.jpg", sequence.image_filename_pattern);
    EXPECT_EQ("groundtruth_rect.txt", sequence.groundtruth_rect);
}
