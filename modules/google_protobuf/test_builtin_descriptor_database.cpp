// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.
//
// This file makes extensive use of RFC 3092.  :)
//
// src/google/protobuf/descriptor_database_unittest.cc

#include <glog/logging.h>
#include <gtest/gtest.h>

#include <google/protobuf/text_format.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor_database.h>
/*

// Defined in this file.
class DescriptorDatabase;
class SimpleDescriptorDatabase;
class EncodedDescriptorDatabase;
class DescriptorPoolDatabase;
class MergedDescriptorDatabase;
*/
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/message.h>

static void AddToDatabase(SimpleDescriptorDatabase* database, const char* file_text) {
    FileDescriptorProto file_proto;
    EXPECT_TRUE(TextFormat::ParseFromString(file_text, &file_proto));
    database->Add(file_proto);
}

static void ExpectContainsType(const FileDescriptorProto& proto, const std::string& type_name) {
    for (int i = 0; i < proto.message_type_size(); i++) {
        if (proto.message_type(i).name() == type_name) return;
    }
    ADD_FAILURE() << "\"" << proto.name() << "\" did not contain expected type \"" << type_name << "\".";
}

class DescriptorDatabaseTest : public testing::Test {
  public:
    // virtual void SetUp() { AddToDatabase(); }
};