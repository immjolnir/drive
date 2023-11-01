#pragma once

#include <fstream>
#include <istream>
#include <string>

#include <glog/logging.h>

std::string ReadFile(const std::string& name) {
    std::ifstream file(name.c_str());
    GOOGLE_CHECK(file.is_open()) << "Couldn't find file '" << name
                                 << "', please make sure you are running "
                                    "this command from the benchmarks/ "
                                    "directory.\n";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}