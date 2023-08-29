#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include <algorithm>  // sort
#include <array>
#include <string>

using namespace boost::filesystem;

using namespace std;

TEST(boost_filesystem, components_of_a_path) {
    // Accessing components of a path
    /*
    Boost.Filesystem provides additional member functions to verify whether a path contains a specific substring. These
    member functions are:
    - has_root_name(),
    - has_root_directory(),
    - has_root_path(),
    - has_relative_path(),
    - has_parent_path(),
    - has_filename().
    Each member function returns a value of type bool.
    */
    path p{"/tmp/a/b/c.txt"};
    EXPECT_EQ("", p.root_name());
    EXPECT_EQ("/", p.root_directory());
    EXPECT_EQ("/", p.root_path());
    EXPECT_EQ("tmp/a/b/c.txt", p.relative_path());
    EXPECT_EQ("/tmp/a/b", p.parent_path());
    EXPECT_TRUE(p.has_filename());
    EXPECT_EQ("c.txt", p.filename());
}

TEST(boost_filesystem, filename_and_extension) {
    path p{"photo.jpg"};
    // returns "photo" for stem() and ".jpg" for extension().
    EXPECT_EQ("photo", p.stem());
    EXPECT_EQ(".jpg", p.extension());
}

TEST(boost_filesystem, iterate_path) {
    // Iterating over components of a path
    path p{"/tmp/a/b/c.txt"};

    /*
    "/"
    "tmp"
    "a"
    "b"
    "c.txt"
    */
    std::vector<path> vec;
    for (const path& pp : p) {
        // std::cout << pp << '\n';
        vec.emplace_back(pp);
    }
    EXPECT_EQ(5, vec.size());
    EXPECT_EQ(path("tmp"), vec[1]);
    EXPECT_EQ(path("c.txt"), vec[4]);
}

TEST(boost_filesystem, concatenating_path) {
    path p{"/tmp"};
    // Concatenating paths with operator/=
    p /= "a.txt";
    EXPECT_EQ("/tmp/a.txt", p.string());
    /* Besides operator/=, Boost.Filesystem provides the member functions
        remove_filename(),
        replace_extension(),
        make_absolute(),
        make_preferred()
     to modify paths.
     */
}

TEST(boost_filesystem, concatenating_path2) {
    path p{"/tmp"};
    // Concatenating paths with operator /
    path res = p / "a" / "a.txt";
    EXPECT_EQ("/tmp/a/a.txt", res.string());
}

TEST(boost_filesystem, list_file) {
    std::vector<directory_entry> files;  // to save the filename in a vector.
    std::copy(directory_iterator(path(".")), directory_iterator(), back_inserter(files));
    for (auto f : files) {
        std::cout << f << std::endl;
    }
}
