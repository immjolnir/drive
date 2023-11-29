# drive

> Create the project from the template project [modern-cpp-template](https://github.com/filipdutescu/modern-cpp-template).


## VSEngine2

```
$ cd VSSystem
$ file -i VSTimer.h
VSTimer.h: text/x-c++; charset=iso-8859-1

$ iconv -f GB2312 -t UTF-8 < VSTimer.h > VSTimer.h

$ file -i VSTimer.h 
VSTimer.h: inode/x-empty; charset=binary
```

For
```
 -i, --mime
         Causes the file command to output mime type strings rather than the more traditional human readable ones.  Thus it may say ‘text/plain; charset=us-ascii’ rather than “ASCII text”.
```

VSTimer.h 变成了空文件。

就是说 `iconv -f $encoding -t UTF-8 < $srcfile > $destfile` 中， srcfile 与 destfile 不能是相同的。

## mlap(Machine Learning Algorithm Principle and Programming Practice)
- See https://blog.51cto.com/u_14122493/5751515

- file: ISO-8859 to UTF-8
```
$ file -i k-means.cpp
$ iconv -f GB2312 -t UTF-8 -o k-means.cpp k-means.cpp
```
这种方法不会导致 k-means.cpp是空的。可行.

```
find . -type f -exec bash -c 'iconv -f GB2312 -t UTF-8 -o {} {}' \;
```
or
```
find . -type f -exec bash -c 't="/tmp/dest"; mkdir -p "$t/`dirname {}`"; iconv -f iso-8859-1 -t utf-8 "{}" > "$t/{}"' \;
```





## VOT(Visual Object Tracking (VOT) challenge)



## Useful link
- [Code Coverage](https://www.jedsharpsoftware.com/c++/2020/09/16/CPP-CodeCoverage.html)
  - [gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) is a utility which is part of the GNU Compiler Collection (GCC) suite and which allows to generate the exact number of times that the instructions of a program have been executed. 

  - [lcov](https://ltp.sourceforge.net/coverage/lcov.php) is a tool that allows you to graphically render the results acquired via the gcov application. 

  - CMake, that no longer needs presentation especially in the world of open source, will be the build system tool that we will use.

