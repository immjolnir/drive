/*
    dynamic_cast conversion

    Safely converts pointers and references to classes up, down, and sideways along the inheritance hierarchy.

    https://en.cppreference.com/w/cpp/language/dynamic_cast

    if (Derived* d = dynamic_cast<Derived*>(b1); d != nullptr)
    {
        std::cout << "downcast from b1 to d successful\n";
        d->name(); // safe to call
    }
*/
#include <gtest/gtest.h>

struct Base {
    virtual ~Base() {}

    const std::string signature = "Base";
};

struct Foo : Base {
    virtual std::string name() { return "Foo"; }

    const std::string signature = "Foo";
};

struct Bar : Base {
    virtual std::string name() { return "Bar"; }

    const std::string signature = "Bar";
};

TEST(dynamic_cast, downcast_pointer_failed) {
    Base* base = new Base;
    bool success_to_foo = false;
    bool success_to_bar = false;
    if (auto* inst = dynamic_cast<Foo*>(base)) {
        success_to_foo = true;
        EXPECT_EQ("Foo", inst->signature);
    }
    EXPECT_FALSE(success_to_foo);

    if (auto* inst = dynamic_cast<Bar*>(base)) {
        success_to_bar = true;
        EXPECT_EQ("Bar", inst->signature);
    }
    EXPECT_FALSE(success_to_bar);
    // Base instance cannot downcast to its derives.
}

TEST(dynamic_cast, downcast_pointer_success) {
    Base* foo = new Foo;
    bool success_to_foo = false;
    bool success_to_bar = false;
    if (auto* inst = dynamic_cast<Foo*>(foo)) {
        success_to_foo = true;
        EXPECT_EQ("Foo", inst->signature);
    }
    EXPECT_TRUE(success_to_foo);

    if (auto* inst = dynamic_cast<Bar*>(foo)) {
        success_to_bar = true;
        EXPECT_EQ("Bar", inst->signature);
    }
    EXPECT_FALSE(success_to_bar);
    // Base instance but created by its derives, can downcast to its derives too.
}

TEST(dynamic_cast, downcast_reference) {
    Foo real_foo;
    Base& foo = real_foo;
    // error: could not convert 'inst' from 'Foo' to 'bool'
    // if (auto& inst = dynamic_cast<Foo&>(foo)) {

    auto& inst_foo = dynamic_cast<Foo&>(foo);
    EXPECT_EQ("Foo", inst_foo.signature);

    // Caution
    // It compiled successfully, but failed at run time with the following error:
    // unknown file: Failure
    // C++ exception with description "std::bad_cast" thrown in the test body.
    try {
        // Check the exception too.
        // https://stackoverflow.com/questions/23270078/test-a-specific-exception-type-is-thrown-and-the-exception-has-the-right-propert
        auto& inst_bar = dynamic_cast<Bar&>(foo);
        EXPECT_EQ("Bar", inst_bar.signature);
    } catch (std::bad_cast const& err) {
        EXPECT_EQ(err.what(), std::string("std::bad_cast"));
    } catch (...) {
        FAIL() << "unexpected exceptions";
    }
}

TEST(dynamic_cast, downcast_reference_base) {
    Base real_base;
    Base& base = real_base;
    // error: could not convert 'inst' from 'Foo' to 'bool'
    // if (auto& inst = dynamic_cast<Foo&>(foo)) {

    // auto& inst_foo = dynamic_cast<Foo&>(base);
    // EXPECT_EQ("Foo", inst_foo.signature);

    // Caution
    // It compiled successfully, but failed at run time with the following error:
    // unknown file: Failure
    // C++ exception with description "std::bad_cast" thrown in the test body.
    try {
        // Check the exception too.
        // https://stackoverflow.com/questions/23270078/test-a-specific-exception-type-is-thrown-and-the-exception-has-the-right-propert
        auto& inst_bar = dynamic_cast<Bar&>(base);
        EXPECT_EQ("Bar", inst_bar.signature);
    } catch (std::bad_cast const& err) {
        EXPECT_EQ(err.what(), std::string("std::bad_cast"));
    } catch (...) {
        FAIL() << "unexpected exceptions";
    }
}