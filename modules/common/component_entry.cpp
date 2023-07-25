#include "component_entry.h"

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

static void dump_flags() {
    std::vector<google::CommandLineFlagInfo> flags;
    google::GetAllFlags(&flags);

    std::map<std::string, std::vector<google::CommandLineFlagInfo>> flags_by_file;
    for (auto& flag : flags) {
        flags_by_file[flag.filename].emplace_back(flag);
    }

    for (auto& entry : flags_by_file) {
        std::sort(entry.second.begin(), entry.second.end(), [](auto&& a, auto&& b) { return a.name < b.name; });
    }
    std::cout << "Flags: \n";
    for (auto& entry : flags_by_file) {
        std::cout << entry.first << ":" << std::endl;
        for (auto& flag : entry.second) {
            std::cout << "\t" << flag.name << "=" << flag.current_value << "(type:" << flag.type
                      << ", default=" << flag.default_value << ")" << std::endl;
        }
    }
}

int ComponentEntry::run(int argc, char** argv) {
    // https://rpg.ifi.uzh.ch/docs/glog.html
    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    // Initialize Google's flag library
    google::ParseCommandLineFlags(&argc, &argv, true);

    dump_flags();
}
