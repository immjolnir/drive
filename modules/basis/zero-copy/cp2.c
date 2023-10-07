#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    char *source_path, *dest_path;
    int source, dest;
    struct stat stat_source;

    if (argc > 1) {
        source_path = argv[1];
    } else {
        source_path = "sendfile.in.tmp";
    }
    if (argc > 2) {
        dest_path = argv[2];
    } else {
        dest_path = "sendfile.out.tmp";
    }

    source = open(source_path, O_RDONLY);
    assert(source != -1);

    dest = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(dest != -1);

    // get file status
    assert(fstat(source, &stat_source) != -1);

    // this copying is done within the kernel -- zero copy.
    assert(sendfile(dest, source, 0, stat_source.st_size) != -1);
    assert(close(source) != -1);
    assert(close(dest) != -1);
    return EXIT_SUCCESS;
}
