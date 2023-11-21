#include "proto/tutorial/addressbook.pb.h"

#include <gtest/gtest.h>
#include <fstream>

TEST(Addressbook, setter_and_getter) {
    proto::tutorial::Person person;
    person.set_name("Scott");  // single optional field
    person.set_id(1);
    person.set_email("scott@gmail.com");

    auto* phones = person.mutable_phones();
    proto::tutorial::Person::PhoneNumber phone1;
    phone1.set_number("abc");
    phone1.set_type(::proto::tutorial::Person_PhoneType_MOBILE);  // ::proto::tutorial::Person_PhoneType
    phones->Add(std::move(phone1));                               // repeated field

    auto phone2 = person.add_phones();
    phone2->set_number("123");
    phone2->set_type(::proto::tutorial::Person_PhoneType_WORK);  // ::proto::tutorial::Person_PhoneType

    proto::tutorial::AddressBook address_book;
    address_book.mutable_people()->Add(std::move(person));  // repeated Person

    EXPECT_EQ(1, address_book.people_size());
    EXPECT_EQ("Scott", address_book.people(0).name());
    EXPECT_EQ(1, address_book.people(0).id());
    EXPECT_EQ("scott@gmail.com", address_book.people(0).email());
    EXPECT_EQ(2, address_book.people(0).phones_size());
}
