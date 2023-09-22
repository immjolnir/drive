# Linker
What's the defference between: `-fpic` and `-fpie`?

from [gnu link options](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html), we we can know:
* default linker(internal linker)

* bfd linker

* gold linker

* LLVM lld liner

* Modern Linker(mold)

```
$ gcc --help=common | grep use-ld
  -fuse-ld=bfd                Use the bfd linker instead of the default linker.
  -fuse-ld=gold               Use the gold linker instead of the default linker.
  -fuse-ld=lld                Use the lld LLVM linker instead of the default linker.
  -fuse-ld=mold               Use the Modern linker (MOLD) linker instead of the default linker.
```

- Check the default linker?
  - https://stackoverflow.com/questions/28688484/actual-default-linker-script-and-settings-gcc-uses

According to [collect2](https://gcc.gnu.org/onlinedocs/gccint/Collect2.html), "The program collect2 works by linking the program once and looking through the linker output file for symbols with particular names indicating they are constructor functions". So it's used to make linking possible.

```
$ strace -o forked.log -s 1024 -f -e trace=/exec gcc hello.c.s  -o hello_gcc

$ grep 'ld' forked.log 
2153  execve("/usr/libexec/gcc/x86_64-redhat-linux/4.8.5/collect2", ["/usr/libexec/gcc/x86_64-redhat-linux/4.8.5/collect2", "--build-id", "--no-add-needed", "--eh-frame-hdr", "--hash-style=gnu", "-m", "elf_x86_64", "-dynamic-linker", "/lib64/ld-linux-x86-64.so.2", "-o", "hello_gcc", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crt1.o", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crti.o", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/crtbegin.o", "-L/usr/lib/gcc/x86_64-redhat-linux/4.8.5", "-L/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64", "-L/lib/../lib64", "-L/usr/lib/../lib64", "-L/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../..", "/tmp/ccyl36jf.o", "-lgcc", "--as-needed", "-lgcc_s", "--no-as-needed", "-lc", "-lgcc", "--as-needed", "-lgcc_s", "--no-as-needed", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/crtend.o", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crtn.o"], 0x17b9da0 /* 61 vars */) = 0
2154  execve("/usr/bin/ld", ["/usr/bin/ld", "--build-id", "--no-add-needed", "--eh-frame-hdr", "--hash-style=gnu", "-m", "elf_x86_64", "-dynamic-linker", "/lib64/ld-linux-x86-64.so.2", "-o", "hello_gcc", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crt1.o", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crti.o", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/crtbegin.o", "-L/usr/lib/gcc/x86_64-redhat-linux/4.8.5", "-L/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64", "-L/lib/../lib64", "-L/usr/lib/../lib64", "-L/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../..", "/tmp/ccyl36jf.o", "-lgcc", "--as-needed", "-lgcc_s", "--no-as-needed", "-lc", "-lgcc", "--as-needed", "-lgcc_s", "--no-as-needed", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/crtend.o", "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crtn.o"], 0x7fff14226a98 /* 61 vars */) = 0
```

- ` gcc -static -o empty empty.c -Wl,--verbose`

- `readelf -W -l empty`

- `$ gcc -Wl,-verbose`

- `$ ld --verbose`

## [Mold: A Modern Linker](https://github.com/rui314/mold)
For details, please see the [design notes](https://github.com/rui314/mold/blob/main/docs/design.md).
