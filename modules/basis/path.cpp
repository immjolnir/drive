#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include <algorithm>  // sort
#include <array>
#include <string>

using namespace boost::filesystem;

using namespace std;

TEST(path, list_file) {
    std::vector<directory_entry> files;  // to save the filename in a vector.
    std::copy(directory_iterator(path(".")), directory_iterator(), back_inserter(files));
    for (auto f : files) {
        std::cout << f << std::endl;
    }
}
