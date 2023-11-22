#include "proto/tutorial/addressbook.pb.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/message.h>

#include <string>

// using the reflect to retrieve the fields
TEST(reflection, test) {
    proto::tutorial::Person person;

    person.set_name("Obama");
    person.set_id(1234);
    person.set_email("1234@qq.com");

    proto::tutorial::Person::PhoneNumber* phone1 = person.add_phones();
    phone1->set_number("110");
    phone1->set_type(proto::tutorial::Person::MOBILE);

    proto::tutorial::Person::PhoneNumber* phone2 = person.add_phones();
    phone2->set_number("119");
    phone2->set_type(proto::tutorial::Person::HOME);

    // error: invalid conversion from 'const google::protobuf::Descriptor*' to 'google::protobuf::Descriptor*'
    // [-fpermissive] google::protobuf::Descriptor* msg_descriptor = person.GetDescriptor();
    const google::protobuf::Descriptor* msg_descriptor = person.GetDescriptor();

    for (int i = 0; i < msg_descriptor->field_count(); ++i) {
        const google::protobuf::FieldDescriptor* fd_descriptor = msg_descriptor->field(i);

        std::string name = fd_descriptor->name();
        std::cout << "Field name: " << name.c_str() << std::endl;
    }

    const google::protobuf::Reflection* msg_reflection = person.GetReflection();
    const google::protobuf::FieldDescriptor* name_field = msg_descriptor->FindFieldByName("name");
    if (name_field != nullptr) {
        std::string val = msg_reflection->GetString(person, name_field);
        std::cout << "Name= " << val << std::endl;
    }

    const google::protobuf::FieldDescriptor* id_field = msg_descriptor->FindFieldByName("id");
    if (id_field != nullptr) {
        int val = msg_reflection->GetInt32(person, id_field);
        std::cout << "id= " << val << std::endl;
    }
}

TEST(reflection, descriptor) {
    google::protobuf::DescriptorPool pool;
    google::protobuf::DynamicMessageFactory factory(&pool);

    google::protobuf::FileDescriptorProto person_file;
    proto::tutorial::Person::descriptor()->file()->CopyTo(&person_file);
    ASSERT_TRUE(pool.BuildFile(person_file) != NULL);
    // std::cout << person_file << std::endl;
}