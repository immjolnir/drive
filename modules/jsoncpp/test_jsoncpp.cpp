#include <gtest/gtest.h>

#include <json/json.h>
#include <string>

// Example from https://stackoverflow.com/questions/46203948/reading-array-of-the-json-in-jsoncpp
// $ python3 -mjson.tool t.json
std::string json_text = R"(
    {
        "return": {
            "status": 200,
            "message": "Accepted"
        },
        "entries": [
            {
                "messageid": 185002992,
                "message": "CplusItsGood",
                "status": 1,
                "statustext": "test",
                "sender": "1234567",
                "receptor": "123456789",
                "date": 1234,
                "cost": 140
            },
            {
                "messageid": 185002993,
                "message": "CplusItsGood",
                "status": 1,
                "statustext": "prod",
                "sender": "1234567",
                "receptor": "123456789",
                "date": 456,
                "cost": 141
            }
        ]
    }
)";

/* Arrays have their own methods. These methods also work for null.

   Some of them are similar to C++ STL's vectors:

    - ArrayIndex size() const;
    - bool empty() const;
    - void clear();
    - void resize(ArrayIndex size);
    - Value& operator[](ArrayIndex index);
    - Value& operator[](int index);
    - const Value& operator[](const ArrayIndex index) const;
    - const Value& operator[](const int index) const;
*/
TEST(jsoncpp, array_reading) {
    Json::Reader reader;
    Json::Value root;

    reader.parse(json_text, root, false);

    auto entries = root["entries"];
    EXPECT_FALSE(entries.empty());
    EXPECT_EQ(2, entries.size());

    {
        Json::Value entry = entries[0];
        EXPECT_EQ(185002992, entry["messageid"].asUInt());
        EXPECT_EQ("test", entry["statustext"].asString());
    }
    {
        Json::Value entry = entries[1];
        EXPECT_EQ(185002993, entry["messageid"].asUInt());
        EXPECT_EQ("prod", entry["statustext"].asString());
    }
}
