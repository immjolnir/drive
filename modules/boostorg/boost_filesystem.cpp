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

    EXPECT_EQ(path("c"), p.stem());  // 不用先获取 filename(), 也可以直接拿到它的stem
    EXPECT_EQ(path(".txt"), p.extension());

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

TEST(boost_filesystem, remove_unexist_file) {
    std::string unexist_filename = "/x/y/z";
    boost::filesystem::path filepath{unexist_filename};

    EXPECT_EQ("/x/y/z", filepath.native());  // 删除一个不存在的文件，竟然是成功的. 难道，它只在乎最终状态?
    boost::system::error_code err_code;
    boost::filesystem::remove(filepath, err_code);
    EXPECT_EQ(err_code, boost::system::errc::success);
    EXPECT_EQ("Success", err_code.message());
}


TEST(boost_filesystem, remove_root_privilage_file) {
    std::string unexist_filename = "/proc";
    boost::filesystem::path filepath{unexist_filename};

    EXPECT_EQ("/proc", filepath.native());  // 删除一个不存在的文件，竟然是成功的. 难道，它只在乎最终状态?
    boost::system::error_code err_code;
    boost::filesystem::remove(filepath, err_code);
    EXPECT_EQ(err_code, boost::system::errc::permission_denied); // 13
    EXPECT_EQ("Permission denied", err_code.message());
}