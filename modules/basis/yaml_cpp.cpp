#include <yaml-cpp/yaml.h>

#include <gtest/gtest.h>
#include <string>

TEST(YAML_CPP, ConfigFile) {
    YAML::Node config = YAML::LoadFile("resources/config.yaml");

    EXPECT_TRUE(config["lastLogin"]);
    EXPECT_FALSE(config["dummy_key"]);
    const std::string username = config["username"].as<std::string>();
    const std::string password = config["password"].as<std::string>();
    EXPECT_EQ("Anonymous", username);
    EXPECT_EQ("anonymous", password);
    EXPECT_TRUE(config["times"]);
    int times = config["times"].as<int>();
    EXPECT_EQ(101, times);

    // std::ofstream fout("config.yaml");
    // fout << config;
}

TEST(YAML_CPP, Sequence) {
    YAML::Node node = YAML::Load("[1, 2, 3]");
    EXPECT_TRUE(node.Type() == YAML::NodeType::Sequence);
    EXPECT_TRUE(node.IsSequence());  // a shortcut!
}
