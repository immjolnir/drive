#include "demo_project/employee.h"
#include "common/component_entry.h"

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>

#include <iostream>

DECLARE_bool(debug_stereo);
DECLARE_string(bag_file);
DECLARE_string(front_left_topic);

DEFINE_string(bag_file, "",
              "input bag file name, used to access bag data in offline replay, "
              "can also be parsed to search calibration date");
DEFINE_bool(debug_stereo, false, "whether to enable logging for debugging stereo");
DEFINE_string(front_left_topic, "/front_left_camera/image_color/compressed", "topic for left camera");

char* safe_strerror_r(int result, char* buf, int err, size_t buflen) {
    if (result != 0) {
        snprintf(buf, buflen, "unknown error: %d", err);
    }
    return buf;
}

char* safe_strerror_r(char* result, char* buf, int err, size_t buflen) { return result; }

std::string errno_to_string(int e) {
    std::array<char, 1024> buf;
    return safe_strerror_r(::strerror_r(e, buf.data(), buf.size()), buf.data(), e, buf.size());
}

bool no_empty_validator(const char* flagname, const std::string& val) {
    if (val.empty()) {
        printf("Invalid value for --%s (must not be empty)\n", flagname);
        return false;
    }
    return true;
}

bool file_validator(const char* flagname, const std::string& path) {
    if (path.empty()) {
        return true;
    }
    FILE* f = ::fopen(path.c_str(), "rb");
    if (f == nullptr) {
        int e = errno;
        printf("Invalid value for --%s: %s (not a readable file, reason: %s, %d)\n", flagname, path.c_str(),
               errno_to_string(e).c_str(), e);
        return false;
    }
    ::fclose(f);
    return true;
}

bool file_validator_no_empty(const char* flagname, const std::string& path) {
    if (!no_empty_validator(flagname, path)) {
        return false;
    }
    // Now just delegate to the one that does allow it to be empty (but we
    // already checked if its not empty, so this one will now do only what
    // we want).
    return file_validator(flagname, path);
}

DEFINE_validator(bag_file, &file_validator_no_empty);

// $ ./examples/employee --bag_file Makefile
int main(int argc, char* argv[]) {
    ComponentEntry entry(argv[0]);
    entry.run(argc, argv);

    Employee sample(1, "Joe", "Blow");
    std::cout << sample << std::endl;
    return 0;
}
