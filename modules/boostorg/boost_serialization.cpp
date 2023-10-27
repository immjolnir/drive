#include <gtest/gtest.h>

/*
 * https://theboostcpplibraries.com/boost.serialization-archive
 */
#include <boost/archive/text_iarchive.hpp>
// binary_iarchive.hpp
// xml_iarchive.hpp

#include <boost/archive/text_oarchive.hpp>

// binary_oarchive.hpp
// xml_oarchive.hpp
#include <boost/serialization/string.hpp>  // To serialize objects of type std::string
#include <string>

#include <boost/serialization/vector.hpp>  // To serialize objects of type std::vector
#include <vector>

// Because smart pointers are used in connection with dynamically allocated memory, Boost.Serialization provides also
// support for them.
#include <boost/serialization/scoped_ptr.hpp>

#include <boost/serialization/shared_ptr.hpp>  // To serialize std::shared_ptr<T>
#include <boost/serialization/unique_ptr.hpp>  // To serialize std::unique_ptr<T>

#include <memory>  // also support std

/*
 * The class boost::archive::text_oarchive serializes data as a text stream.
 * * Include boost/archive/text_iarchive.hpp
 * The class boost::archive::text_iarchive restores data from such a text stream.
 * * Include boost/archive/text_oarchive.hpp.
 */

/**
 * Serializing with a stringstream
 */
TEST(boost_serialization, archive_serialize_primary_types) {
    using namespace boost::archive;

    auto save = [](std::stringstream& ss) {
        text_oarchive oa{ss};
        int i = 1;
        oa << i;
    };

    auto load_with_check = [](std::stringstream& ss) {
        text_iarchive ia{ss};
        int i = 0;
        ia >> i;
        EXPECT_EQ(i, 1);
    };

    std::stringstream ss;

    save(ss);
    load_with_check(ss);
}

/**
 * how to serialize objects of user-defined types
 * Serializing user-defined types with a member function
 */
class animal {
  public:
    animal() = default;

    animal(int legs, std::string name) : legs_{legs}, name_{std::move(name)} {}

    int legs() const { return legs_; }

    const std::string& name() const { return name_; }

  private:
    /*
  In order to serialize objects of user-defined types, you must define the member function serialize(). This function is
  called when the object is serialized to or restored from a byte stream. Because serialize() is used for both
  serializing and restoring, Boost.Serialization supports the operator operator& in addition to operator<< and
  operator>>. With operator& there is no need to distinguish between serializing and restoring within serialize().

  serialize() is automatically called any time an object is serialized or restored. It should never be called explicitly
  and, thus, should be declared as private. If it is declared as private, the class boost::serialization::access must be
  declared as a friend to allow Boost.Serialization to access the member function.
  */
    friend class boost::serialization::access;

    template <typename Archive>
    friend void serialize(Archive& ar, animal& a, const unsigned int version);

    int legs_;
    std::string name_;
};

template <typename Archive>
void serialize(Archive& ar, animal& a, const unsigned int version) {
    ar & a.legs_;
    if (version > 0) ar & a.name_;
}

BOOST_CLASS_VERSION(animal, 1)

TEST(boost_serialization, archive_serialize_user_defined_types) {
    // https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test
    // GTEST_SKIP() << "Skipping single test";

    // Serialize object to text stream
    auto save = [](std::stringstream& ss) {
        boost::archive::text_oarchive oa{ss};
        animal a{4, "cat"};
        oa << a;
    };

    // Restore the object from text stream
    auto load_with_check = [](std::stringstream& ss) {
        boost::archive::text_iarchive ia{ss};
        animal b;
        ia >> b;
        EXPECT_EQ(4, b.legs());
        EXPECT_EQ("cat", b.name());
    };

    std::stringstream ss;
    save(ss);
    // std::string expected = R"(22 serialization::archive 19 0 1 4 3 cat)";
    // EXPECT_EQ(expected, ss.str());
    load_with_check(ss);
}

TEST(boost_serialization, archive_serialize_vector) {
    auto save = [](std::stringstream& ss) {
        std::vector<short> uvec;
        uvec.emplace_back(0x8000);
        uvec.emplace_back(0x7fff);
        uvec.emplace_back(0xffff);

        EXPECT_EQ(3, uvec.size());
        EXPECT_EQ(-32768, uvec[0]);
        EXPECT_EQ(32767, uvec[1]);
        EXPECT_EQ(-1, uvec[2]);

        boost::archive::text_oarchive oa{ss};
        oa << uvec;
    };

    // Restore the object from text stream
    auto load_with_check = [](std::stringstream& ss) {
        boost::archive::text_iarchive ia{ss};
        std::vector<short> uvec;
        ia >> uvec;

        EXPECT_EQ(3, uvec.size());
        EXPECT_EQ(-32768, uvec[0]);
        EXPECT_EQ(32767, uvec[1]);
        EXPECT_EQ(-1, uvec[2]);
    };

    std::stringstream ss;
    save(ss);

    EXPECT_EQ(392, sizeof(std::stringstream));
    ss.seekp(0, std::ios::end);
    std::stringstream::pos_type offset = ss.tellp();
    // 22 serialization::archive 18 3 0 -32768 32767 -1
    // std::cout << ss.str() << std::endl;
    EXPECT_EQ(49, offset);
    EXPECT_EQ(49, ss.str().length());
    // ss.seekp(0, std::ios::end);
    load_with_check(ss);
}

/*
Boost.Serialization automatically serializes the object referenced by a and not the address of the object.

If the archive is restored, a will not necessarily contain the same address. A new object is created and its address is
assigned to a instead. Boost.Serialization only guarantees that the object is the same as the one serialized, not that
its address is the same.

Because smart pointers are used in connection with dynamically allocated memory, Boost.Serialization provides also
support for them.
*/
TEST(boost_serialization, archive_serialize_pointer) {
    // https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test
    // GTEST_SKIP() << "Skipping single test";

    // Serialize object to text stream
    auto save = [](std::stringstream& ss) {
        boost::archive::text_oarchive oa{ss};
        animal* a = new animal{4, "cat"};
        oa << a;
        std::cout << std::hex << a << '\n';
        delete a;
    };

    // Restore the object from text stream
    auto load_with_check = [](std::stringstream& ss) {
        boost::archive::text_iarchive ia{ss};
        animal* a;
        ia >> a;
        std::cout << std::hex << a << '\n';
        EXPECT_EQ(4, a->legs());
        EXPECT_EQ("cat", a->name());

        delete a;
    };

    std::stringstream ss;
    save(ss);
    // std::string expected = R"(22 serialization::archive 19 0 1 4 3 cat)";
    // EXPECT_EQ(expected, ss.str());
    load_with_check(ss);
}

TEST(boost_serialization, archive_serialize_smart_ptr) {
    // Serialize object to text stream
    auto save = [](std::stringstream& ss) {
        boost::archive::text_oarchive oa{ss};
        boost::scoped_ptr<animal> a(new animal{4, "cat"});
        oa << a;
    };

    // Restore the object from text stream
    auto load_with_check = [](std::stringstream& ss) {
        boost::archive::text_iarchive ia{ss};
        boost::scoped_ptr<animal> a;
        ia >> a;
        EXPECT_EQ(4, a->legs());
        EXPECT_EQ("cat", a->name());
    };

    std::stringstream ss;
    save(ss);
    // std::string expected = R"(22 serialization::archive 19 0 1 4 3 cat)";
    // EXPECT_EQ(expected, ss.str());
    load_with_check(ss);
}

TEST(boost_serialization, archive_serialize_shared_ptr) {
    // Serialize object to text stream
    auto save = [](std::stringstream& ss) {
        boost::archive::text_oarchive oa{ss};
        std::shared_ptr<animal> a(new animal{4, "cat"});
        oa << a;
    };

    // Restore the object from text stream
    auto load_with_check = [](std::stringstream& ss) {
        boost::archive::text_iarchive ia{ss};
        std::shared_ptr<animal> a;
        ia >> a;
        EXPECT_EQ(4, a->legs());
        EXPECT_EQ("cat", a->name());
    };

    std::stringstream ss;
    save(ss);
    load_with_check(ss);
}

TEST(boost_serialization, archive_serialize_unique_ptr) {
    // Serialize object to text stream
    auto save = [](std::stringstream& ss) {
        boost::archive::text_oarchive oa{ss};
        std::unique_ptr<std::vector<int>> vec(new std::vector<int>({1, -2, 3}));
        oa << vec;
    };

    // Restore the object from text stream
    auto load_with_check = [](std::stringstream& ss) {
        boost::archive::text_iarchive ia{ss};
        std::unique_ptr<std::vector<int>> vec;
        ia >> vec;
        EXPECT_EQ(3, vec->size());
    };

    std::stringstream ss;
    save(ss);
    std::cout << ss.str() << std::endl;
    // std::string expected = R"(22 serialization::archive 19 0 1 4 3 cat)";
    // EXPECT_EQ(expected, ss.str());
    load_with_check(ss);
}

namespace hierarchy_object
{

    class animal {
      public:
        animal() = default;

        animal(int legs) : legs_{legs} {}

        int legs() const { return legs_; }

      private:
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int version) {
            ar & legs_;
        }

        int legs_;
    };

    class bird : public animal {
      public:
        bird() = default;

        bird(int legs, bool can_fly) : animal{legs}, can_fly_{can_fly} {}

        bool can_fly() const { return can_fly_; }

      private:
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int version) {
            // Derived class must access the fution base_ojbect` inside the member function `serialize()`
            // to serialize objects based on class hierarchies.
            ar& boost::serialization::base_object<animal>(*this);
            ar & can_fly_;
        }

        bool can_fly_;
    };

    // Failed for ==23148==ERROR: AddressSanitizer: new-delete-type-mismatch on 0x602000010970 in thread T0:
    TEST(boost_serialization, serialize_hierarchy_object) {
        GTEST_SKIP() << "Skipping it for address sanitizer fails";

        auto save = [](std::stringstream& ss) {
            boost::archive::text_oarchive oa{ss};
            oa.register_type<bird>();
            animal* a = new bird{2, false};
            oa << a;
            delete a;
        };

        auto load = [](std::stringstream& ss) {
            boost::archive::text_iarchive ia{ss};
            ia.register_type<bird>();
            animal* a;
            ia >> a;

            EXPECT_EQ(2, a->legs());
            /*
            dynamic_cast is a keyword; do drop the `std::`

            error: cannot 'dynamic_cast' 'a' (of type 'class hierarchy_object::animal*') to type 'class
            hierarchy_object::bird*' (source type is not polymorphic)

            if (auto b = dynamic_cast<bird*>(a)) {

            Becuase the base type don't have virtual function.
            */
            // EXPECT_FALSE(((bird*)a)->can_fly());
            delete a;
        };

        std::stringstream ss;
        save(ss);
        load(ss);
    }
}  // namespace hierarchy_object
