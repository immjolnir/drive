#include "proto/common/timestamp.pb.h"
#include "proto/tutorial/obstacle.pb.h"

#include <gtest/gtest.h>

#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

TEST(Obstacle, message_to_json_string) {
    proto::common::Timestamp timestamp;
    timestamp.set_seconds(1);
    timestamp.set_nanos(2);

    proto::tutorial::ObstacleArray obstacle_array;
    obstacle_array.mutable_timestamp()->CopyFrom(timestamp);

    proto::tutorial::Quaternion rotation;
    rotation.set_qw(0.999999896540799);
    rotation.set_qx(6.2160608428610731e-05);
    rotation.set_qy(-1.3763930389949054e-06);
    rotation.set_qz(0.000450613532471245);

    // [libprotobuf FATAL /usr/include/google/protobuf/repeated_field.h:1716] CHECK failed: (index) < (current_size_):
    // auto* calib_refine0 = obstacle_array.mutable_calib_refine(0);
    auto* calib_refine0 = obstacle_array.add_calib_refine();
    calib_refine0->set_sensor_name("front_radar");
    calib_refine0->mutable_rotation()->CopyFrom(rotation);

    std::string actual;  // binary format
    google::protobuf::util::JsonOptions json_options;
    json_options.add_whitespace = true;
    EXPECT_TRUE(google::protobuf::util::MessageToJsonString(obstacle_array, &actual, json_options).ok());

    // From the out, we can see that
    // the attribue: `calib_refine` defined in proto will be converted to `calibRefine` in output json string
    std::cout << actual << std::endl;
}

TEST(Obstacle, json_string_to_message) {
    std::string text = R"(
                       {
                        "timestamp": {
                         "seconds": "1",
                         "nanos": 2
                        },
                        "calibRefine": [
                         {
                          "sensorName": "front_radar",
                          "rotation": {
                           "qw": 0.999999896540799,
                           "qx": 6.2160608428610731e-05,
                           "qy": -1.3763930389949054e-06,
                           "qz": 0.000450613532471245
                          }
                         }
                        ]
                       }
    )";

    proto::tutorial::ObstacleArray obstacle_array;
    EXPECT_TRUE(google::protobuf::util::JsonStringToMessage(text, &obstacle_array).ok());

    EXPECT_TRUE(obstacle_array.has_timestamp());
    EXPECT_EQ(1, obstacle_array.timestamp().seconds());
    EXPECT_EQ(2, obstacle_array.timestamp().nanos());

    EXPECT_EQ(1, obstacle_array.calib_refine_size());
    const auto& calib_refine0 = obstacle_array.calib_refine(0);

    EXPECT_EQ("front_radar", calib_refine0.sensor_name());
    EXPECT_TRUE(calib_refine0.has_rotation());
    EXPECT_DOUBLE_EQ(0.999999896540799, calib_refine0.rotation().qw());
    EXPECT_DOUBLE_EQ(6.2160608428610731e-05, calib_refine0.rotation().qx());
    EXPECT_DOUBLE_EQ(-1.3763930389949054e-06, calib_refine0.rotation().qy());
    EXPECT_DOUBLE_EQ(0.000450613532471245, calib_refine0.rotation().qz());

    // From this case, we can see that
    // the attribute name: `calibRefine` of the json string will be converted to `calib_refeine`,
    // which is alwayed defined in proto.
}