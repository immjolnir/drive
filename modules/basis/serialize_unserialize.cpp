/*
 * See https://isocpp.org/wiki/faq/serialization#serialize-simple-types
 * Here is how to serialize/unserialize simple types in
 * - human-readable (“text”) format
 * - non-human-readable (“binary”) format
 *
 * Solutions in https://stackoverflow.com/questions/234724/is-it-possible-to-serialize-and-deserialize-a-class-in-c
 */

#include <sstream>

#include <gtest/gtest.h>

// ================ Solution 1  ======================
struct Bits {
    int a;
    int b;
    int c;

    friend std::ostream& operator<<(std::ostream& out, const Bits& obj) {
        out << obj.a << "," << obj.b << "," << obj.c;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Bits& obj) {
        char sep;
        in >> obj.a >> sep >> obj.b >> sep >> obj.c;
        return in;
    }
};

TEST(old_cpp_ways, human_readable_text) {
    Bits val{1, 2, 3};
    std::stringstream ss0;
    ss0 << val;
    std::string raw_string = R"(1,2,3)";
    EXPECT_EQ(raw_string, ss0.str());

    std::stringstream ss1;
    ss1 << "1,2,3";

    Bits val1;
    ss1 >> val1;

    EXPECT_EQ(1, val1.a);
    EXPECT_EQ(2, val1.b);
    EXPECT_EQ(3, val1.c);
}

// ================ Solution 2  ======================
#include <boost/pfr.hpp>

struct some_person {
    std::string name;
    unsigned birth_year;
};

/**
 * Boost.PFR is a C++14 library for a very basic reflection. It gives you access to structure elements by index and
 * provides other std::tuple like methods for user defined types without macro or boilerplate code:
 *
 * https://www.boost.org/doc/libs/1_75_0/doc/html/boost_pfr.html
 * https://github.com/boostorg/pfr
 *
 */
TEST(boost, fpr) {
    some_person val{"Edgar Allan Poe", 1809};

    std::string person_name = boost::pfr::get<0>(val);
    unsigned birth_year = boost::pfr::get<1>(val);

    EXPECT_EQ("Edgar Allan Poe", person_name);
    EXPECT_EQ(1809, birth_year);

    std::stringstream ss0;
    ss0 << boost::pfr::io(val);
    // https://en.cppreference.com/w/cpp/language/string_literal
    std::string raw_string = R"({"Edgar Allan Poe", 1809})";

    EXPECT_EQ(raw_string, ss0.str());

    std::stringstream ss1(raw_string);
    some_person nobody;
    ss1 >> boost::pfr::io(nobody);
    EXPECT_EQ("Edgar Allan Poe", nobody.name);
    EXPECT_EQ(1809, nobody.birth_year);
    // std::cin >> boost::pfr::io(val);
    // std::cout << boost::pfr::io(val) << '\n';
}

// ================ Solution 3  ======================
/*
 * https://theboostcpplibraries.com/boost.serialization-archive
 */
#include <boost/archive/text_iarchive.hpp>
// binary_iarchive.hpp
// xml_iarchive.hpp

#include <boost/archive/text_oarchive.hpp>
// binary_oarchive.hpp
// xml_oarchive.hpp
// #include <boost/serialization/string.hpp>

class animal {
  public:
    animal() = default;

    animal(int legs, std::string name) : legs_{legs}, name_{std::move(name)} {}

    int legs() const { return legs_; }

    const std::string& name() const { return name_; }

  private:
    friend class boost::serialization::access;

    template <typename Archive>
    friend void serialize(Archive& ar, animal& a, const unsigned int version);

    int legs_;
    std::string name_;
};

template <typename Archive>
void serialize(Archive& ar, animal& a, const unsigned int version) {
    ar& a.legs_;
    if (version > 0) ar& a.name_;
}

BOOST_CLASS_VERSION(animal, 1)

TEST(boost, serialization) {
    std::stringstream ss;
    boost::archive::text_oarchive oa{ss};
    animal a{4, "cat"};
    oa << a;
    std::string expected = R"(22 serialization::archive 19 0 1 4 3 cat)";
    EXPECT_EQ(expected, ss.str());

    boost::archive::text_iarchive ia{ss};
    animal b;
    ia >> b;
    EXPECT_EQ(4, b.legs());
    EXPECT_EQ("cat", b.name());
}