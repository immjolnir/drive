# Tools

## cppcheck

## Include What You Use.

- Depends
  - libncurses5
  - libncurses5-dev

  - libllvm6.0
  - llvm-6.0
  - llvm-6.0-dev
  - llvm-6.0-runtime
  - llvm-6.0-tools

  - clang-6.0
  - clang-format-6.0
  - clang-tidy-6.0
  - libclang-6.0-dev

```
$ clang++ --version
clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ git checkout clang_6.0

$ cmake -G "Unix Makefiles" -DIWYU_LLVM_ROOT_PATH=/usr/lib/llvm-6.0 ..

$ make

$ ./include-what-you-use --version
include-what-you-use 0.10 (git:a1878c4) based on clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
```
or 
```
$ apt search include-what-you-use
Sorting... Done
Full Text Search... Done
iwyu/jammy 8.17-1 amd64
  Analyze #includes in C and C++ source files

$ apt show iwyu
Package: iwyu
Version: 8.17-1
Priority: extra
Section: universe/devel
Origin: Ubuntu
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Original-Maintainer: LLVM Packaging Team <pkg-llvm-team@lists.alioth.debian.org>
Bugs: https://bugs.launchpad.net/ubuntu/+filebug
Installed-Size: 13.2 MB
Depends: libc6 (>= 2.34), libclang-cpp13 (>= 1:13.0.0), libllvm13, libstdc++6 (>= 11), clang | clang-4.0 | clang-5.0 | clang-6.0 | clang-7 | clang-8 | clang-9 | clang-10 | clang-11 | clang-12 | clang-13, python3
Homepage: https://include-what-you-use.org/
Download-Size: 1,219 kB
APT-Sources: http://cn.archive.ubuntu.com/ubuntu jammy/universe amd64 Packages
Description: Analyze #includes in C and C++ source files
 "Include what you use" means this: for every symbol (type, function variable,
 or macro) that you use in foo.cc, either foo.cc or foo.h should #include a .h
 file that exports the declaration of that symbol. The include-what-you-use
 tool is a program that can be built with the clang libraries in order to
 analyze #includes of source files to find include-what-you-use violations,
 and suggest fixes for them.
 .
 The main goal of include-what-you-use is to remove superfluous #includes.
 It does this both by figuring out what #includes are not actually needed for
 this file (for both .cc and .h files), and replacing #includes with
 forward-declares when possible.
```

### Usage

- Run againt on a single file
  ```
  $ ./include-what-you-use  test.cpp 

    test.cpp should add these lines:

    test.cpp should remove these lines:
    - #include "a.h"  // lines 2-2
    - #include "b.h"  // lines 3-3

    The full include-list for test.cpp:
    #include <iostream>  // for operator<<, endl, basic_ostream, cout, ostream
    ---
  ```
  Where the test.cpp as below:
  ```c++
    #include <iostream>
    #include "a.h"
    #include "b.h"

    int main() {
        std::cout << "h" << std::endl;
    }
  ```

- CMAKE
  - https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_INCLUDE_WHAT_YOU_USE.html
    ```
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "path to your executable include-what-you-use")
    ```
  Then run it as before, it will outputs the hints.

- Makefile
  ```
  $ make -D CXX=path-to-your-iwyu > iwyu.txt

  $ python include-what-you-use/fix_includes.py < iwyu.txt
    >>> Fixing #includes in '../common-master/common/include/periodics/scheduler.h'
    >>> Fixing #includes in '../common-master/common/src/periodics/scheduler.cpp'
    >>> Fixing #includes in '../common-master/ipc_benchmark/test_deque_and_circular_buffer.cpp'
    >>> Fixing #includes in '../common-master/ipc_benchmark/proto_benchmark.cpp'
    >>> Fixing #includes in '../common-master/ipc_benchmark/read_shared_log.cpp'
    >>> Fixing #includes in '../common-master/ipc_benchmark/play_shared_log.cpp'
  ```
  这会修改 `#include` directive, 也会使用 forward declaration.
  
- https://github.com/include-what-you-use/include-what-you-use/issues/608
  - example: https://github.com/gflags/gflags/blob/master/src/gflags.h.in#L84

有时候相同的符号在很多文件中都是定义，可以使用 map 的方法指定引用哪个文件。map 文件 的方法可以参考文档 IWYUMappings。 然后在运行 IWYU 时指定 map 文件：
```
$ make -B -k CXX=include-what-you-use CFLAGS=" -Xiwyu --mapping_file=mapfile"
```
有时候 IWYU 也会犯错，它会错误的认为可以删除某些头文件的引用。这种情况下可以使用 IWYU pragmas 控制 IWYU 的行为。

- IMYU Mappings: https://github.com/include-what-you-use/include-what-you-use/blob/master/boost-1.64-all.imp
- IMYU pragmas: https://github.com/include-what-you-use/include-what-you-use/blob/master/iwyu_preprocessor.h

### Reference
- https://developer.huawei.com/consumer/cn/forum/topic/0203923292356080575
- https://www.fluentcpp.com/2021/01/01/include-what-you-use/
- https://blog.csdn.net/u013473725/article/details/110122813
- https://xsyr.github.io/%E7%BC%96%E7%A8%8B/c/c++/2013/10/30/include-what-you-use.html

