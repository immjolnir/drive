# Zereo-Copy: sendfile 

## sendfile

```
#include <sys/sendfile.h>

ssize_t sendfile(int out_fd, int in_fd, off_t *_Nullable offset, size_t count);
```

- `sendfile()` copies data between one file descriptor and another. Because this copying is done within the kernel, `sendfile()` is more efficient than the combination of read(2) and write(2), which would require transferring data to and from user space.



For instance:
```
$ dd if=/dev/urandom of=sendfile.in.tmp bs=1K count=10M
10485760+0 records in
10485760+0 records out
10737418240 bytes (11 GB, 10 GiB) copied, 44.8099 s, 240 MB/s

$ time strace ./cp2 sendfile.in.tmp sendfile.in.tmp3
execve("./cp2", ["./cp2", "sendfile.in.tmp", "sendfile.in.tmp3"], 0x7fff3438a1c0 /* 81 vars */) = 0
brk(NULL)                               = 0x5582ad265000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffe399fb4e0) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f67499c1000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=117869, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 117869, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f67499a4000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0i8\235HZ\227\223\333\350s\360\352,\223\340."..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=2216304, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2260560, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f674977c000
mmap(0x7f67497a4000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f67497a4000
mmap(0x7f6749939000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7f6749939000
mmap(0x7f6749991000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x214000) = 0x7f6749991000
mmap(0x7f6749997000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f6749997000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f6749779000
arch_prctl(ARCH_SET_FS, 0x7f6749779740) = 0
set_tid_address(0x7f6749779a10)         = 131099
set_robust_list(0x7f6749779a20, 24)     = 0
rseq(0x7f674977a0e0, 0x20, 0, 0x53053053) = 0
mprotect(0x7f6749991000, 16384, PROT_READ) = 0
mprotect(0x5582abc38000, 4096, PROT_READ) = 0
mprotect(0x7f67499fb000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f67499a4000, 117869)          = 0
openat(AT_FDCWD, "sendfile.in.tmp", O_RDONLY) = 3
openat(AT_FDCWD, "sendfile.in.tmp3", O_WRONLY|O_CREAT|O_TRUNC, 0600) = 4
newfstatat(3, "", {st_mode=S_IFREG|0664, st_size=10737418240, ...}, AT_EMPTY_PATH) = 0
sendfile(4, 3, NULL, 10737418240)       = 2147479552
close(3)                                = 0
close(4)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++

real	0m1.217s
user	0m0.005s
sys	0m1.213s

```

read(3, "(7\7\273j1\321\35\373;P\264f\344\257\214+\31E\330\26\305\336'\224\37 ,\30\310~\313"..., 131072) = 131072
write(4, "(7\7\273j1\321\35\373;P\264f\344\257\214+\31E\330\26\305\336'\224\37 ,\30\310~\313"..., 131072) = 131072
read(3, "<\337\324\v\371\261]\315{\346\363\341&\317r\204\324\317\244\207\r\10\335\312YG\0160\332-\355\315"..., 131072) = 131072
write(4, "<\337\324\v\371\261]\315{\346\363\341&\317r\204\324\317\244\207\r\10\335\312YG\0160\332-\355\315"..., 131072) = 131072
read(3, "\327\30\333\310\347\337F\v\234\301\r\253\210\310\265\"\200#+\242\251\206\200`\247\0\nv\367\267(\327"..., 131072) = 131072
write(4, "\327\30\333\310\347\337F\v\234\301\r\253\210\310\265\"\200#+\242\251\206\200`\247\0\nv\367\267(\327"..., 131072) = 131072

$ time cp sendfile.in.tmp sendfile.in.tmp0

real	0m10.987s
user	0m0.060s
sys	0m9.022s
