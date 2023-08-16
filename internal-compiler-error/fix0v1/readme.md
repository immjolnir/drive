
# internal compiler error: Segmentation fault

```
/usr/include/c++/7/bits/hashtable_policy.h:820:61: internal compiler error: Segmentation fault
       using size_type = typename __hashtable_base::size_type;
                                                             ^
Please submit a full bug report,
with preprocessed source if appropriate.
See <file:///usr/share/doc/gcc-7/README.Bugs> for instructions.
```
or
```
c++: internal compiler error: Segmentation fault (program cc1plus)
Please submit a full bug report,
with preprocessed source if appropriate.
See <file:///usr/share/doc/gcc-7/README.Bugs> for instructions.
```

## issue-case
```
                     a.h    <--    b.h
                      \             ^
                       \            |
myclass_size.cpp-->  myclass.h      |
                         ^          |
                         |   +------+
                         |   |
                     myclass.cpp
```
* Run
```
$ make src/myclass.i

$ ll CMakeFiles/myclass_size.dir/src/myclass_size.cpp.i
-rw-r--r-- 1 root root 741081 Aug 16 15:27 CMakeFiles/myclass_size.dir/src/myclass_size.cpp.i

# 这个文件中只有一个 struct A 的定义。
```



