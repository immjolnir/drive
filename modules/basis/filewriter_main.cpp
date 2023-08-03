#include "filewriter.h"

int main() {
    hacking::file_writer << "xxx"
                         << "bb"
                         << "\n";

    // template argument deduction/substitution failed:
    // note:   couldn't deduce template parameter '_Tp'
    // hacking::file_writer << 124 << 'a' << std::endl;
    hacking::file_writer << 124 << 'a' << "\n";
    hacking::file_writer.flush();  // flush it explicitly

    return 0;
}