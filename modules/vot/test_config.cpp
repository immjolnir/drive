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
    std::cout << ret.first << std::endl;
}