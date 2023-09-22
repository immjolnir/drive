#include "io.h"

#include <gtest/gtest.h>
#include <iostream>

using namespace my;

TEST(myio, example) {
    std::cout << "tt" << std::endl;
    if (mout._buf != nullptr) {
        std::cout << "not null" << std::endl;
    }

    std::cout << __ioinit._S_refcount << std::endl;
    EXPECT_EQ(2, __ioinit._S_refcount);

    mout.add("Hello IO!");

    std::cout << mout.to_string() << std::endl;
}

/** We can print the definition order of the variables: mout and __ioinit.

* https://stackoverflow.com/questions/30714646/how-can-i-differentiate-static-functions-with-nm-or-readelf-output-in-c

 $ readelf -s test_io  | grep ioinit
   347: 00000000000592a0     1 OBJECT  LOCAL  DEFAULT   28 _ZN2myL8__ioinitE

$ readelf -s test_io  | grep mout
   222: 0000000000059150    16 OBJECT  GLOBAL DEFAULT   28 _ZN2my4moutE
 *

 2.

 Give following commands in sequence ((gdb) prompt come by default as you go on typing commands)

 (gdb) maint print symbols filename
 (gdb) maint print psymbols filename
 (gdb) maint print msymbols filename

[1882] B 0x59150 _ZN2my4moutE section .bss  my::mout  crtstuff.c

[1888] b 0x592a0 _ZN2myL8__ioinitE section .bss  my::__ioinit  test_io.cpp
[1889] b 0x592a1 _ZStL8__ioinit section .bss  std::__ioinit  test_io.cpp

- B: Global bss symbol.
- b: Local bss symbol.
*/