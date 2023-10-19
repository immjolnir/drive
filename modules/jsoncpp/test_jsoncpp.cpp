#include <gtest/gtest.h>

#include <json/json.h>
#include <string>

// https://github.com/open-source-parsers/jsoncpp/blob/master/example/readFromString/readFromString.cpp
/**
 * \brief Parse a raw string into Value object using the CharReaderBuilder
 * class, or the legacy Reader class.
 * Example Usage:
 * $g++ readFromString.cpp -ljsoncpp -std=c++11 -o readFromString
 * $./readFromString
 * colin
 * 20
 */
TEST(jsoncpp, readFromString_OldWay) {
    const std::string rawJson = R"({"Age": 20, "Name": "colin"})";
    JSONCPP_STRING err;
    Json::Value root;
    Json::Reader reader;
    reader.parse(rawJson, root);

    const std::string name = root["Name"].asString();
    const int age = root["Age"].asInt();
    EXPECT_EQ("colin", name);
    EXPECT_EQ(20, age);
}

TEST(jsoncpp, readFromString_NewWay) {
    const std::string rawJson = R"({"Age": 20, "Name": "colin"})";

    const auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    Json::Value root;

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {
        FAIL() << "This cannot happen! err: " << err;
    }

    const std::string name = root["Name"].asString();
    const int age = root["Age"].asInt();

    EXPECT_EQ("colin", name);
    EXPECT_EQ(20, age);
}

// https://github.com/open-source-parsers/jsoncpp/blob/master/example/readFromStream/readFromStream.cpp
/** \brief Parse from stream, collect comments and capture error info.
 * Example Usage:
 * $g++ readFromStream.cpp -ljsoncpp -std=c++11 -o readFromStream
 * $./readFromStream
 * // comment head
 * {
 *    // comment before
 *    "key" : "value"
 * }
 * // comment after
 * // comment tail
 */
TEST(jsoncpp, readFromString_Succ) {
    const std::string rawJson = R"(
        // comment head
        {
            // comment before
            "key" : "value"
            // comment after
        }// comment tail
    )";
    Json::Value root;
    // std::ifstream ifs;
    // ifs.open(argv[1]);

    std::stringstream ifs;
    ifs << rawJson;

    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING err;
    if (!Json::parseFromStream(builder, ifs, &root, &err)) {
        FAIL() << "This cannot happen! err: " << err;
    }
    const std::string val = root["key"].asString();
    EXPECT_EQ("value", val);
}

TEST(jsoncpp, readFromString_Fail) {
    const std::string rawJson = R"(
        {
            1: "value"
        }
    )";
    Json::Value root;
    // std::ifstream ifs;
    // ifs.open(argv[1]);

    std::stringstream ifs;
    ifs << rawJson;

    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING err;
    if (!Json::parseFromStream(builder, ifs, &root, &err)) {
        EXPECT_TRUE(!err.empty());
        // * Line 3, Column 13\n  Missing '}' or object member name\n
    } else {
        // not reach this branch
        FAIL() << "This cannot happen! err: " << err;
    }
}

// https://github.com/open-source-parsers/jsoncpp/blob/master/example/stringWrite/stringWrite.cpp
// \brief Write a Value object to a string.
TEST(jsoncpp, writeString_OldWay) {
    Json::Value data;
    data["number"] = 1;

    Json::Value root;
    root["action"] = "run";
    root["data"] = data;

    Json::FastWriter writer;
    const std::string json_str = writer.write(root);

    const std::string expected = R"(
    {
        "action" : "run",
        "data" : 
        {
            "number" : 1
        }
    }
    )";
    EXPECT_TRUE(!json_str.empty());
}

TEST(jsoncpp, writeString_NewWay) {
    Json::Value data;
    data["number"] = 1;

    Json::Value root;
    root["action"] = "run";
    root["data"] = data;

    Json::StreamWriterBuilder builder;
    const std::string json_str = Json::writeString(builder, root);

    const std::string expected = R"(
    {
        "action" : "run",
        "data" : 
        {
            "number" : 1
        }
    }
    )";
    EXPECT_TRUE(!json_str.empty());
}

TEST(jsoncpp, writeStream) {
    Json::Value root;
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    root["Name"] = "robin";
    root["Age"] = 20;
    // writer->write(root, &std::cout);

    std::stringstream ss;
    writer->write(root, &ss);
    /* expected result:
    {
        "Age" : 20,
        "Name" : "robin"
    }
    */
    std::cout << ss.str() << std::endl;
    // Check if a stringstream is empty or null (C++)
    EXPECT_TRUE(ss.tellp() != std::streampos(0));
}

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
TEST(jsoncpp, Array) {
    // Example from https://stackoverflow.com/questions/46203948/reading-array-of-the-json-in-jsoncpp
    // $ python3 -mjson.tool t.json
    const std::string json_text = R"(
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