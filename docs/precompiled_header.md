
Precompiled headers basically improve build times by reducing the amount of code compiling each time a build command is invoked.


# [The Meson Build System: Using Precompiled Headers](https://gcc.gnu.org/onlinedocs/gcc/Precompiled-Headers.html)


Often large projects have many header files that are included in every source file. The time the compiler takes to process these header files over and over again can account for nearly all of the time required to build the project. To make builds faster, GCC allows you to precompile a header file.

To create a precompiled header file, simply compile it as you would any other file, if necessary using the -x option to make the driver treat it as a C or C++ header file. 

The name searched for is the name specified in the #include with ‘.gch’ appended. If the precompiled header file cannot be used, it is ignored.
For instance, if you have `#include "all.h"`, and you have `all.h.gch` in the same directory as `all.h`, then the precompiled header file is used if possible, and the original header is used otherwise.

Alternatively, you might decide to put the precompiled header file in a directory and use `-I` to ensure that directory is searched before (or instead of) the directory containing the original header. Then, if you want to check that the precompiled header file is always used, you can put a file of the same name as the original header in this directory containing an `#error` command.
```
#error "You won't see it if everything is normal."

```

This also works with `-include`. So yet another way to use precompiled headers, good for projects not designed with precompiled header files in mind, is to simply take most of the header files used by a project, include them from another header file, precompile that header file, and `-include` the precompiled header. If the header files have guards against multiple inclusion, they are skipped because they’ve already been included (in the precompiled header).

A precompiled header file can be used only when these conditions apply:
- Only one precompiled header can be used in a particular compilation.

- Each of the following options must be the same when building and using the precompiled header: `-fexceptions`

- A precompiled header cannot be used once the first C token is seen. You can have preprocessor directives before a precompiled header; you cannot include a precompiled header from inside another header.

- The precompiled header file must be produced for the same language as the current compilation. You cannot use a C precompiled header for a C++ compilation.

- The precompiled header file must have been produced by the same compiler binary as the current compilation is using.

- Some other command-line options starting with `-f`, `-p`, or `-O` must be defined in the same way as when the precompiled header was generated.
At present, it’s not clear which options are safe to change and which are not; the safest choice is to use exactly the same options when generating and using the precompiled header. The following are known to be safe:
  - `-fmessage-length=`
  - `-fpreprocessed `
  - `-fsched-interblock`
  - `-fsched-spec`
  - `-fsched-spec-load`
  - `-fsched-spec-load-dangerous`
  - `-fsched-verbose=number`
  - `-fschedule-insns`
  - `-fvisibility=`
  - `-pedantic-errors`

## [Precompiled headers](https://thiblahute.github.io/jpakkane.github.io/Precompiled-headers.html)

Parsing header files of system libraries is surprisingly expensive. A typical source file has less than one thousand lines of code. In contrast the headers of large libraries can be tens of thousands of lines. This is especially problematic with C++, where header-only libraries are common and they may contain extremely complex code. This makes them slow to compile.

Precompiled headers are a tool to mitigate this issue. Basically what they do is parse the headers and then serialize the compiler's internal state to disk. The downside of precompiled headers is that they are tricky to set up. Meson has native support for precompiled headers, but using them takes a little work.

A precompiled header file is relatively simple. It is a header file that contains #include directives for the system headers to precompile. Here is a C++ example.

    #include<vector>
    #include<string>
    #include<map>
In Meson, precompiled header files are always per-target. That is, the given precompiled header is used when compiling every single file in the target. Due to limitations of the underlying compilers, this header file must not be in the same subdirectory as any of the source files. It is strongly recommended that you create a subdirectory called pch in the target directory and put the header files (and nothing else) there.

Toggling the usage of precompiled headers
If you wish to compile your project without precompiled headers, you can change the value of the pch option by passing -Db_pch=false argument to Meson at configure time or later with mesonconf. You can also toggle the use of pch in a configured build directory with the GUI tool. You don't have to do any changes to the source code. Typically this is done to test whether your project compiles cleanly without pch (that is, checking that its #includes are in order) and working around compiler bugs.

Using precompiled headers with GCC and derivatives
Once you have a file to precompile, you can enable the use of pch for a give target with a pch keyword argument. As an example, here's how you would use it with a C binary.

executable('myexe', sources : sourcelist, c_pch : 'pch/myexe_pch.h')
You should note that your source files must not include the file myexe_pch.h and you must not add the pch subdirectory to your search path. Meson will make the compiler include the pch with compiler options. If you want to disable pch (because of, say, compiler bugs), it can be done entirely on the build system side with no changes to source code.

You can use precompiled headers on any build target. If your target has multiple languages, you can specify multiple pch files like this.

executable('multilang', sources : srclist,
           c_pch : 'pch/c_pch.h', cpp_pch : 'pch/cpp_pch.h'])



----

- man g++

```
       -include file
           Process file as if "#include "file"" appeared as the first line of the primary source file.  However, the first directory searched for file is the preprocessor's working directory
           instead of the directory containing the main source file.  If not found there, it is searched for in the remainder of the "#include "..."" search chain as normal.

           If multiple -include options are given, the files are included in the order they appear on the command line.

       -H  Print the name of each header file used, in addition to other normal activities.  Each name is indented to show how deep in the #include stack it is.  Precompiled header files are also
           printed, even if they are found to be invalid; an invalid precompiled header file is printed with ...x and a valid one with ...! .

```

+	# IMPORTANT: Make sure to edit this so it has all the required flags!
+	g++ -c -x c++-header stdafx.cpp -o stdafx.h.gch
+
+    $ g++ -c -x c++-header precomp.cpp -o precomp.h.gch -I /usr/include/eigen3
+    $ time g++ -I /usr/include/eigen3 -I /usr/include test_geometry.cpp -o test_geometry -L /usr/lib/x86_64-linux-gnu/ -lgtest -lgtest_main
+    $ time g++ -I /usr/include/eigen3 -I /usr/include -include precomp.h.gch test_geometry.cpp -o test_geometry -L /usr/lib/x86_64-linux-gnu/ -lgtest -lgtest_main



$ time g++ -I /usr/include/eigen3 -I /usr/include test_geometry.cpp -o test_geometry -L /usr/lib/x86_64-linux-gnu/ -lgtest -lgtest_main 


real	0m1.507s
user	0m1.384s
sys	0m0.116s


$ time g++ -I /usr/include/eigen3 -I /usr/include -include precomp.h test_geometry.cpp -o test_geometry1 -L /usr/lib/x86_64-linux-gnu/ -lgtest -lgtest_main

real	0m1.490s
user	0m1.358s
sys	0m0.132s

$ time g++ -I /usr/include/eigen3 -I /usr/include -I . -include precomp.h test_geometry.cpp -o test_geometry4 -L /usr/lib/x86_64-linux-gnu/ -lgtest -lgtest_main  1> /dev/null 2>&1

real	0m1.441s
user	0m1.304s
sys	0m0.136s


A subtle tip about the file extension that tripped me up, because I wasn't paying close enough attention: the .gch extension is added to the precompiled file's full name; it doesn't replace .h. If you get it wrong, the compiler won't find it and silently does not work.

precomp.h => precomp.h.gch

Not:

precomp.h => precomp.gch

Use GCC's -H to check if it's finding/using it.

$ g++ -c precomp.h -o precomp.h.gch -I /usr/include/eigen3


- https://stackoverflow.com/questions/58841/precompiled-headers-with-gcc
The ! means that the compiler was able to use the precompiled header. An x means it was not able to use it. Using the appropriate compiler flags is crucial. I took off the -H and ran some speed tests. The precompiled header had an improvement from 14 seconds to 11 seconds. Not bad, but not great.

buildtime: real=2.01 user=1.85 sys=0.14 [ g++ -O2 -H -Wall -std=c++14 -I /usr/include/eigen3 -I /usr/include -c -o test_geometry.o test_geometry.cpp ]
buildtime: real=1.32 user=1.22 sys=0.09 [ g++ -O2 -H -Wall -std=c++14 -I /usr/include/eigen3 -I /usr/include -include precomp.h -c -o test_geometry.o test_geometry.cpp ]


- https://clang.llvm.org/docs/PCHInternals.html

Cpp Best Practice 

- https://github.com/cpp-best-practices/cppbestpractices/blob/master/08-Considering_Performance.md

- https://pvs-studio.com/en/blog/posts/cpp/1053/

## pic vs pie
- https://stackoverflow.com/questions/28119365/what-are-the-differences-comparing-pie-pic-code-and-executable-on-64-bit-x86-pl

> PIC and PIE generally mean almost the same thing, while PIC emphasizes on libraries and PIE emphasize on executables. In old days, programs were position dependent, i.e., after compile, the program has to be loaded into, and run from, a particular absolute address on memory.
