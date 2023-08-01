#include <fmt/core.h>

#include <iostream>
#include <string>

int main() {
    int i = 18;
    fmt::print("{:6}\n", i);   // width 6
    fmt::print("{:06}\n", i);  // 0-prefixed
    fmt::print("\n");
    fmt::print("{:>6}\n", i);   // align right
    fmt::print("{:*>6}\n", i);  // align right
    fmt::print("{:*^6}\n", i);  // center
    fmt::print("{:*<6}\n", i);  // align left

    std::string res = fmt::format("{:06}.png", i);
    std::cout << "res=" << res << std::endl;

    // ‘-std=c++20’
    //  fmt::format_string pattern = FMT_STRING("{:06}");
    //  std::string res0 = fmt::format(pattern, i);
    //  std::cout << "res0=" << res0 << std::endl;
}
