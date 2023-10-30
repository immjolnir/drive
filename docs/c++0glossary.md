# Glossry

## code bloat
代码膨胀：指软件中存在过多的冗余代码，导致程序体积过大、运行速度变慢或难以维护。

Catch it from https://github.com/protocolbuffers/protobuf/blob/63f4c503a2762abaec136209ee5b62452ecf206e/src/google/protobuf/repeated_field.h#L934

- The software suffers from code bloat, making it difficult to maintain and update. 该软件存在代码膨胀问题，导致难以维护和更新。

- This leads to bugs, code bloat, and blatant DRY violations. 这导致bug、代码膨胀，并严重违背了DRY原则。

The `inline` keyword has little to do with inlining unfortunately. It's more about placing the function `inline` in the header rather than the inlining optimisation.

If binary size is truely a concern, there are compiler flags such as `-Os` and `-Oz` that will reduce the binary size and also reduce inlining at the cost of speed.


- [Google's bloaty](https://github.com/google/bloaty): Bloaty: a size profiler for binaries

- https://www.reddit.com/r/cpp/comments/wo5lvw/template_code_bloat_how_to_measure_and_what_does/

```
$ ./bloaty example_basic_cat -d inlines -n 100
$ size -A -d ./stripped/example_basic_cat
$ objdump -s -j .rodata ./stripped/example_basic_cat 
```

- Compiler invocation

clang++
```
-ffunction-sections

-flto

-fmerge-functions

-Oz

${ALL_YOUR_SOURCE_FILES}

-fvisibility=hidden

-fforce-emit-vtables

-fwhole-program-vtables

-fstrict-vtable-pointers

-fnew-infallible

-c

-fuse-ld=lld
```

- Linker invocation:
```
clang++

-Wl,--gc-sections

-Wl,--icf=safe

-Wl,-mllvm,-enable-function-merging

-Wl,-mllvm,-machine-outliner-reruns=5

${ALL_YOUR_OBJECT_FILES}

-o exec
```

You'll need to annotate all your exported functions with `__attribute__((visibility("default"))` for `-fvisilibility=hidden` to work.

There's more toggles in llvm for size, I just can't remember off the top of my head at the moment.

If you're not using the latest clang and lld, well... just stop not using the latest clang and lld.

# Least-Significant x bits
- least significant bit (LSB)
The LSB refers to the rightmost bit in a binary number representation. It holds the lowest value in the binary place value system, representing 2^0 or 1. In other words, the LSB is the bit that carries the smallest weight in a binary number.


If there is no decimal point, the rightmost nonzero digit is the `least significant digit`. If there is a decimal point, the rightmost digit is the `least significant digit`, even if it is a 0. All digits between the least and most significant digits are counted as significant.

- How is the LSB utilized in network protocols?

Network protocols often use the LSB for error checking, ensuring data integrity, and even determining packet types. For example, in the internet protocol (IP) header, the LSB of the type of service (ToS) field is used to mark whether the packet should be treated as low delay, high throughput, or high reliability.

- Can I use LSB in error correction techniques?
While the LSB itself is not typically used for error correction, it can be part of a larger error correction strategy. Error correction techniques like Hamming codes, Reed-Solomon codes, or error-correcting memory use additional bits, including parity bits, to detect and correct errors.

- Techopedia Explains Least Significant Bit

Digital data is computed in binary format, and similarly to numerical notation, the right digit is considered the lowest digit whereas the leftmost is considered the highest digit. Due to the positional notation, the least significant bit is also known as the rightmost bit. It is the opposite of the most significant bit, which carries the highest value in a multiple-bit binary number as well as the number which is farthest to the right. In a multi-bit binary number, the significance of a bit decreases as it approaches the least significant bit. Since it is binary, the most significant bit can be either 1 or 0.

When a transmission of binary data is done with the least significant bit first technique, the least significant bit is the one which is transmitted first, followed by other bits of increasing significance. The least significant bit is frequently employed in hash functions, checksums and pseudorandom number generators.

# stub

A `method stub` or simply `stub` in software development is a piece of code used to stand in for some other programming functionality. A `stub` may simulate the behavior of existing code (such as a procedure on a remote machine; such methods are often called mocks) or be a temporary substitute for yet-to-be-developed code. Stubs are therefore most useful in porting, distributed computing as well as general software development and testing.

A `stub` is a routine that doesn't actually do anything other than declaring itself and the parameters it accepts and returning something that is usually the values expected in one of the "happy scenarios" for the caller. Stubs are used commonly as placeholders for implementation of a known interface, where the interface is finalized/known but the implementation is not yet known/finalized. The `stub` contains just enough code to allow it to be compiled and linked with the rest of the program.

