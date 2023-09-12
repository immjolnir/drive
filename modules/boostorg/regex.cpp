
/*
// $ g++ -std=c++14 extract_bag_time.cc  -lboost_regex  -lboost_date_time

* boost::regex_search
* boost::posix_time::from_iso_string
*
*/

#include <gtest/gtest.h>
#include <boost/date_time.hpp>
#include <boost/regex.hpp>  // boost::smatch
#include <string>

// Bag name example: 20200620T093120_a-b-c-1_11.bag
const static boost::regex BAG_NAME_EXPR{R"(^((\d{8})T(\d{6}))_([a-z0-9-]+)_(.*)\.(bag|db)$)"};

// const static boost::regex BAG_NAME_EXPR{R"(^((\d{8})T(\d{6}))_([a-z0-9A-Z-]+)_(.*)\.(bag|db)$)"};

bool parse_bag_name(const std::string& bag_name, boost::posix_time::ptime& bag_time, std::string& vehicle,
                    std::string& offset) {
    boost::smatch what;
    // if (!boost::regex_search(bag_name, what, BAG_NAME_EXPR) || what.size() < 6) {
    if (!boost::regex_search(bag_name, what, BAG_NAME_EXPR)) {
        std::cout << "Not found";
        return false;
    }

    bag_time = boost::posix_time::from_iso_string(what[1]);
    vehicle = what[4];
    offset = what[5];

    return true;
}

std::string getBagTimeFromName(const std::string& bag_name) {
    if (!bag_name.empty()) {
        int pos = bag_name.find_last_of("/");
        boost::posix_time::ptime bag_time_t;
        std::string vehicle, offset;
        if (parse_bag_name(bag_name.substr(pos + 1), bag_time_t, vehicle, offset)) {
            return boost::posix_time::to_iso_string(bag_time_t);
        }
    }
    return std::string();
}

TEST(boost_regex, parse_bag_name) {
    std::string bag_name = "20200620T093120_ab-cd_11.bag";

    boost::posix_time::ptime time;
    std::string vehicle;
    std::string offset;
    EXPECT_TRUE(parse_bag_name(bag_name, time, vehicle, offset));
    EXPECT_EQ("20200620T093120", boost::posix_time::to_iso_string(time));
    EXPECT_EQ("ab-cd", vehicle);
    EXPECT_EQ("11", offset);
}

TEST(boost_regex, parse_bag_name2) {
    std::string bag_name = "20211027T095913_ab-cd-e1_4_1910to1960.db";

    boost::posix_time::ptime time;
    std::string vehicle;
    std::string offset;
    EXPECT_TRUE(parse_bag_name(bag_name, time, vehicle, offset));
    EXPECT_EQ("20211027T095913", boost::posix_time::to_iso_string(time));
    EXPECT_EQ("ab-cd-e1", vehicle);
    EXPECT_EQ("4_1910to1960", offset);
}

TEST(boost_regex, getBagTimeFromName_empty) {
    std::string empty_bag_name;
    EXPECT_TRUE(getBagTimeFromName(empty_bag_name).empty());
}

TEST(boost_regex, getBagTimeFromName) {
    std::string bag_name = "/a/b/c/234/20211027T095913_ab-cd-e1_4_1910to1960.db";

    auto time_str = getBagTimeFromName(bag_name);

    EXPECT_FALSE(time_str.empty());
    EXPECT_EQ("20211027T095913", time_str);
}