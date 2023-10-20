#include <filesystem>  // since c++17
/**
 * https://en.cppreference.com/w/cpp/filesystem/temp_directory_path
 *
- path temp_directory_path()
- path temp_directory_path(std::error_code&ec);
 - A directory suitable for temporary files. The path is guaranteed to exist and to be a directory.

- exists(file_status)
- exists(path)

- is_symlink
  - Checks if the given file status or path corresponds to a symbolic link, as if determined by the POSIX S_IFLNK

- create_symlink
- create_directory_symlink

- read_symlink(path& p):
- read_symlink(path& p, error_code& ec):
  - If the path p refers to a symbolic link, returns a new path object which refers to the target of that symbolic link.
  - It is an error if p does not refer to a symbolic link.

- bool equivalent(p1, p2)
  - Checks whether the paths p1 and p2 resolve to the same file system entity.

- path::compare(path&)
- path::compare(std::string)
  - Compares the lexical representations of the path and another path.
  - Return value
    - A value less than ​0​ if the path is lexicographically less than the given path.
    - A value equal to ​0​ if the path is lexicographically equal to the given path.
    - A value greater than ​0​ if the path is lexicographically greater than the given path.

- path relative( const std::filesystem::path& p, std::error_code& ec );
- path relative( const std::filesystem::path& p, const std::filesystem::path& base = std::filesystem::current_path());
  - Return the path relative to base

 */

#include <boost/filesystem.hpp>
/*
* https://www.boost.org/doc/libs/1_78_0/libs/filesystem/doc/reference.html#unique_path

- path unique_path(const path& model="%%%%-%%%%-%%%%-%%%%");
- path unique_path(const path& model, system::error_code& ec);
  - The unique_path function generates a path name suitable for creating temporary files, including directories.

*/

#include <gtest/gtest.h>
#include <fstream>

namespace fs = std::filesystem;

TEST(std17_filesystem, temp_directory_path) {
    std::error_code ec;

    fs::path tmp_path = fs::temp_directory_path(ec);
    std::cout << "Creating temp directory: " << tmp_path.string() << std::endl;
    EXPECT_TRUE(!tmp_path.string().empty());
    // error_code's bool() operator
    // explicit operator bool() const noexcept { return _M_value != 0; }
    bool has_error = bool(ec);
    EXPECT_FALSE(has_error);
}

TEST(std17_filesystem, equivalent) {
    // hard link equivalency
    fs::path p1 = ".";
    fs::path p2 = fs::current_path();
    EXPECT_TRUE(fs::equivalent(p1, p2));
}

TEST(std17_filesystem, boost_create_temp_file_via_unique_path) {
    // ln [OPTION]... [-T] TARGET LINK_NAME
    boost::filesystem::path target_path = boost::filesystem::unique_path();  // like faf5-6991-e095-7597
    EXPECT_FALSE(boost::filesystem::exists(target_path));
    // Then we can create directory or file with target_path.
    std::cout << "Target Path: " << target_path.string() << std::endl;
}

TEST(std17_filesystem, create_symlink) {
    fs::path tmp_path = fs::temp_directory_path();
    // ln [OPTION]... [-T] TARGET LINK_NAME
    // create temp file with unique_path
    fs::path target = tmp_path / boost::filesystem::unique_path().string();
    fs::path symlink = tmp_path / boost::filesystem::unique_path().string();

    std::ofstream{target.string()};  // create regular file
    fs::create_symlink(target, symlink);

    EXPECT_TRUE(fs::exists(target));
    EXPECT_TRUE(fs::exists(symlink));

    EXPECT_FALSE(fs::is_symlink(target));
    EXPECT_TRUE(fs::is_symlink(symlink));
    fs::path resolved = fs::read_symlink(symlink);
    std::cout << "resolved " << target << " => " << symlink << std::endl;
    // resolved "/tmp/788b-4d47-e874-c156" => "/tmp/3825-5f1a-ed63-1755"
    EXPECT_TRUE(fs::equivalent(target, symlink));
    // Cannot use the compare to tell whether they are pointing to a same file.
    EXPECT_NE(0, target.compare(symlink));

    // Tell wether they are pointing to a same file
    // Determine whether they point to the same file
    fs::path relative_target = fs::relative(target);
    EXPECT_TRUE(fs::equivalent(target, symlink));
    EXPECT_NE(0, relative_target.compare(symlink));
    std::cout << "resolved " << relative_target << " => " << symlink << std::endl;
}
