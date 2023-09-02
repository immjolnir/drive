#include "proto/tutorial/addressbook.pb.h"

#include <gtest/gtest.h>

TEST(Addressbook, write_and_read) {
    proto::tutorial::Person person;
    person.set_name("Scott");
    person.set_id(1);
    person.set_email("scott@gmail.com");

    auto* phones = person.mutable_phones();
    proto::tutorial::Person::PhoneNumber phone1;
    phone1.set_number("abc");
    phone1.set_type(::proto::tutorial::Person_PhoneType_MOBILE);  // ::proto::tutorial::Person_PhoneType
    phones->Add(std::move(phone1));

    auto phone2 = person.add_phones();
    phone2->set_number("123");
    phone2->set_type(::proto::tutorial::Person_PhoneType_WORK);  // ::proto::tutorial::Person_PhoneType

    proto::tutorial::AddressBook address_book;
    address_book.mutable_people()->Add(std::move(person));

    // EXPECT_EQ(1, timestamp.seconds());
    // EXPECT_EQ(2, timestamp.nanos());
}
