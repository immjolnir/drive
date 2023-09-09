
Precompiled headers basically improve build times by reducing the amount of code compiling each time a build
command is invoked.


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


