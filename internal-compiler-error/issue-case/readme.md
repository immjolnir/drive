
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
                     a.h <-- b.h
                      \      /
                       \    /
myclass_size.cpp-->   myclass.h
                         ^
                         |
                     myclass.cpp
```
* Run
```
$ make src/myclass_size.i

$ ll CMakeFiles/myclass_size.dir/src/myclass_size.cpp.i
-rw-rw-r-- 1 root root 741311  8月 16 16:25 CMakeFiles/myclass_size.dir/src/myclass_size.cpp.i

# 这个文件中有 struct A 和 struct B 的定义。
```


