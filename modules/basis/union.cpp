#include <gtest/gtest.h>

/*
A union can have member functions (including constructors and destructors), but not virtual functions.

A union cannot have base classes and cannot be used as a base class.

A union cannot have non-static data members of reference types.

Just like in struct declaration, the default member access in a union is public.
*/

// https://www.geeksforgeeks.org/structures-unions-and-enumerations-in-cpp/
//
// The size of the union is the size of its largest field (or larger if alignment so requires, for example on a SPARC
// machine a union contains a double and a char [17] so its size is likely to be 24 because it needs 64-bit alignment).
// Unions cannot have a destructor.
//

union SPARC {
    double d;
    char arr[17];
};

static_assert(sizeof(SPARC) == 24, "Its size is 24B");

union Foo {
    double d;
    char c;
};

union alignas(64) Bar {
    double d;
    char c;
};

TEST(union, size) {
    EXPECT_EQ(24, sizeof(SPARC));

    // The size of the union is the size of its largest field.
    EXPECT_TRUE(sizeof(Foo) == sizeof(double));
    EXPECT_EQ(8, sizeof(double));
    EXPECT_EQ(8, sizeof(Foo));

    // The size of the union is larger than its largest field for alignment.
    EXPECT_EQ(64, sizeof(Bar));
}

// https://en.cppreference.com/w/cpp/language/union
// The union is at least as big as necessary to hold its largest data member, but is usually not larger.
// The other data members are intended to be allocated in the same bytes as part of that largest member.
// The details of that allocation are implementation-defined,
// but all non-static data members will have the same address (since C++14).
// It is undefined behavior to read from the member of the union that wasn't most recently written.
TEST(union, which_active_field) {
    Foo foo;
    foo.d = 1.0;  // d: is now the active member
    EXPECT_EQ(1.0, foo.d);

    foo.c = 'a';  // c is now the active member
    EXPECT_EQ('a', foo.c);
    // UB(undefined behavior)
    // It shouldnot work any more.
    // EXPECT_EQ(1.0, foo.d);
}

/**
 * All members of a union must reside at the same address, that is guaranteed by the standard.
 *
 * https://stackoverflow.com/questions/33056403/is-it-legal-to-use-address-of-one-field-of-a-union-to-access-another-field
 * https://stackoverflow.com/questions/11373203/accessing-inactive-union-member-and-undefined-behavior
 *
 * Reference from
 * FloatingPoint
 * : /usr/include/gtest/internal/gtest-internal.h
 */
// The data type used to store the actual floating-point number.
struct FloatUnion {
    FloatUnion(float x) { u.value_ = x; }

    union FloatingPointUnion {
        float value_;         // The raw floating-point number.
        std::uint32_t bits_;  // The bits that represent the number.
    };

    void PrintAsScitific() { std::cout << "float(scientific)=" << std::scientific << u.value_ << std::endl; }

    void PrintAsFloat() {
        std::cout << "float =" << std::setprecision(std::numeric_limits<float>::digits10 + 2) << u.value_ << std::endl;
    }

    void PrintAsUint32() { std::cout << "uint32 =" << u.bits_ << std::endl; }

    FloatingPointUnion u;
};

TEST(union, FloatUnion_size) {
    EXPECT_EQ(4, sizeof(FloatUnion));
    EXPECT_EQ(4, sizeof(FloatUnion::FloatingPointUnion));
};

TEST(union, different_members_can_access_a_same_space_n1) {
    FloatUnion fu(-1.0f);
    EXPECT_EQ(0xbf'80'00'00, fu.u.bits_);
    fu.PrintAsScitific();
    fu.PrintAsFloat();
    fu.PrintAsUint32();

    const float f = *reinterpret_cast<const float*>(&fu.u.bits_);
    EXPECT_FLOAT_EQ(-1.0f, f);
}

TEST(union, different_members_can_access_a_same_space_p1) {
    FloatUnion fu(1.0f);
    EXPECT_EQ(0x3f'80'00'00, fu.u.bits_);
    fu.PrintAsScitific();
    fu.PrintAsFloat();
    fu.PrintAsUint32();

    const float f = *reinterpret_cast<const float*>(&fu.u.bits_);
    EXPECT_FLOAT_EQ(1.0f, f);
}

TEST(union, different_members_can_access_a_same_space_pi) {
    // constexpr float kPiFloat = 3.14159265358979323846f;  // float 只能表示到 3.141592741e+00
    FloatUnion fu(3.14159265358979323846f);  // digits10 = 6, 小数部分只能保存到第6位
    EXPECT_EQ(0x40'49'0f'db, fu.u.bits_);
    fu.PrintAsScitific();
    fu.PrintAsFloat();
    fu.PrintAsUint32();

    const float f = *reinterpret_cast<const float*>(&fu.u.bits_);
    EXPECT_FLOAT_EQ(3.1415927f, f);  // 第7位就是 round了
}