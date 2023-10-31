# Protobuf: Google's data interchange format

- Projects
  - [protobuf](https://github.com/protocolbuffers/protobuf)
  - [protoscope](https://github.com/protocolbuffers/protoscope)

- Learning code
  - modules/google_protobuf/

## types
- src/google/protobuf/stubs/port.h
```c++
namespace google {
namespace protobuf {
typedef unsigned int uint;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

static const int32 kint32max = 0x7FFFFFFF;
static const int32 kint32min = -kint32max - 1;
static const int64 kint64max = PROTOBUF_LONGLONG(0x7FFFFFFFFFFFFFFF);
static const int64 kint64min = -kint64max - 1;
static const uint32 kuint32max = 0xFFFFFFFFu;
static const uint64 kuint64max = PROTOBUF_ULONGLONG(0xFFFFFFFFFFFFFFFF);


#ifndef bswap_16
static inline uint16 bswap_16(uint16 x) {
  return static_cast<uint16>(((x & 0xFF) << 8) | ((x & 0xFF00) >> 8));
}
#define bswap_16(x) bswap_16(x)
#endif

#ifndef bswap_32
static inline uint32 bswap_32(uint32 x) {
  return (((x & 0xFF) << 24) |
          ((x & 0xFF00) << 8) |
          ((x & 0xFF0000) >> 8) |
          ((x & 0xFF000000) >> 24));
}
#define bswap_32(x) bswap_32(x)
#endif

#ifndef bswap_64
static inline uint64 bswap_64(uint64 x) {
  return (((x & PROTOBUF_ULONGLONG(0xFF)) << 56) |
          ((x & PROTOBUF_ULONGLONG(0xFF00)) << 40) |
          ((x & PROTOBUF_ULONGLONG(0xFF0000)) << 24) |
          ((x & PROTOBUF_ULONGLONG(0xFF000000)) << 8) |
          ((x & PROTOBUF_ULONGLONG(0xFF00000000)) >> 8) |
          ((x & PROTOBUF_ULONGLONG(0xFF0000000000)) >> 24) |
          ((x & PROTOBUF_ULONGLONG(0xFF000000000000)) >> 40) |
          ((x & PROTOBUF_ULONGLONG(0xFF00000000000000)) >> 56));
}
#define bswap_64(x) bswap_64(x)
#endif
}
}
```


## [TextFormat](https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/text_format.h)
- ParseFromString: convert a human-readable string into a protocol buffer object.
  - `static bool ParseFromString(absl::string_view input, Message* output);`

- PrintToString: convert a protobuf buffer object to a human-readable string.
  - `static bool PrintToString(const Message& message, std::string* output);`
  - Note: output will be cleared prior to printing, and will be left empty even if printing fails. Returns false if printing fails.

## [MessageLite](https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/message_lite.h)
  - ParseFromString

```
  // Parses a protocol buffer contained in a string. Returns true on success.
  // This function takes a string in the (non-human-readable) binary wire
  // format, matching the encoding output by MessageLite::SerializeToString().
  // If you'd like to convert a human-readable string into a protocol buffer
  // object, see google::protobuf::TextFormat::ParseFromString().
  PROTOBUF_ATTRIBUTE_REINITIALIZES bool ParseFromString(absl::string_view data);
```

  - ParsePartialFromString
```
  // Like ParseFromString(), but accepts messages that are missing required fields.
  PROTOBUF_ATTRIBUTE_REINITIALIZES bool ParsePartialFromString(absl::string_view data);
```

  - SerializeToString
```
  // Serialize the message and store it in the given string.  All required
  // fields must be set.
  bool SerializeToString(std::string* output) const;
```

## RepeatedField
- /usr/include/google/protobuf/repeated_field.h

`RepeatedField` is used to represent repeated fields of a primitive type (in other words, everything except strings and nested Messages).
Most users will not ever use a `RepeatedField` directly; they will use the get-by-index, set-by-index, and add accessors that are generated for all repeated fields.

```c++
template <typename Element>
class RepeatedField final {

private:
  // A note on the representation here (see also comment below for
  // RepeatedPtrFieldBase's struct Rep):
  //
  // We maintain the same sizeof(RepeatedField) as before we added arena support
  // so that we do not degrade performance by bloating memory usage. Directly
  // adding an arena_ element to RepeatedField is quite costly. By using
  // indirection in this way, we keep the same size when the RepeatedField is
  // empty (common case), and add only an 8-byte header to the elements array
  // when non-empty. We make sure to place the size fields directly in the
  // RepeatedField class to avoid costly cache misses due to the indirection.
  int current_size_;
  int total_size_;
  struct Rep {
    Arena* arena;
    // Here we declare a huge array as a way of approximating C's "flexible
    // array member" feature without relying on undefined behavior.
    Element elements[(std::numeric_limits<int>::max() - 2 * sizeof(Arena*)) /
                     sizeof(Element)];
  };

  // If total_size_ == 0 this points to an Arena otherwise it points to the
  // elements member of a Rep struct. Using this invariant allows the storage of
  // the arena pointer without an extra allocation in the constructor.
  void* arena_or_elements_;

  typedef void InternalArenaConstructable_;

  // Move the contents of |from| into |to|, possibly clobbering |from| in the
  // process.  For primitive types this is just a memcpy(), but it could be
  // specialized for non-primitive types to, say, swap each element instead.
  void MoveArray(Element* to, Element* from, int size);

  // Copy the elements of |from| into |to|.
  void CopyArray(Element* to, const Element* from, int size);

  // Internal helper to delete all elements and deallocate the storage.
  void InternalDeallocate(Rep* rep, int size) {
    if (rep != NULL) {
      Element* e = &rep->elements[0];
      if (!std::is_trivial<Element>::value) {
        Element* limit = &rep->elements[size];
        for (; e < limit; e++) {
          e->~Element(); // 调用每个对象的 dtor.
        }
      }
      if (rep->arena == NULL) { // 只有在不使用  arena 时，才真正 delete 这部分 new 的空间.
#if defined(__GXX_DELETE_WITH_SIZE__) || defined(__cpp_sized_deallocation)
        const size_t bytes = size * sizeof(*e) + kRepHeaderSize;
        ::operator delete(static_cast<void*>(rep), bytes);
#else
        ::operator delete(static_cast<void*>(rep));
#endif
      }
    }
};
```
- Ctors: 4 kinds
  - `RepeatedField();`: default ctor
  
  - `RepeatedField(const RepeatedField& other);`: Copy ctor

  - `explicit RepeatedField(Arena* arena);`: Special ctor
  - `template <typename Iter> RepeatedField(Iter begin, const Iter& end);`: Special ctor

  - `RepeatedField& operator=(const RepeatedField& other);`: copy assignment operator
  - `RepeatedField(RepeatedField&& other) noexcept;`: move ctor
  - `RepeatedField& operator=(RepeatedField&& other) noexcept;`: move assignment operator

- Dtor:
  - `~RepeatedField();`

- Space related funcs
  - `void Reserve(int new_size);`: Reserve space to expand the field to at least the given size.  If the array is grown, it will always be at least doubled in size.
  - `void Resize(int new_size, const Element& value);`: 
    - Like STL resize.  Uses value to fill appended elements. 
    - Like Truncate() if new_size <= size(), otherwise this is O(new_size - size()).

  - `void Truncate(int new_size);`: Resize the RepeatedField to a new, smaller size.  This is O(1).

  - `Element* mutable_data();`
  - `const Element* data() const;`
    - Gets the underlying array, this pointer is possible invalidated by any add or remove operation.

  - `size_t SpaceUsedExcludingSelfLong() const;`: Returns the number of bytes used by the repeated field, excluding `sizeof(*this)`.
    ```c++
    int SpaceUsedExcludingSelf() const {
      return internal::ToIntSize(SpaceUsedExcludingSelfLong());
    }
    ```

- Add elements
  - `void Set(int index, const Element& value);`
  - `void Add(const Element& value);`: Appends a new element and return a pointer to it. The new element is uninitialized if |Element| is a POD type.

  - `Element* Add();`
  - `template <typename Iter> void Add(Iter begin, Iter end);`: Append elements in the range [begin, end) after reserving the appropriate number of elements.

- Access elements
  - `const Element& Get(int index) const;`
  - `Element* Mutable(int index);`

  - `const Element& operator[](int index) const { return Get(index); }`
  - `Element& operator[](int index) { return *Mutable(index); }`

  - `const Element& at(int index) const;`
  - `Element& at(int index);`

- Remove elements from the array
  - `void RemoveLast();`: Remove the last element in the array.
  - `Clear()`;

- Helper methods
  - `bool empty() const;`
  - `int size() const;`


- Details
```c++
template <typename Element>
inline RepeatedField<Element>::RepeatedField(const RepeatedField& other)
    : current_size_(0), total_size_(0), arena_or_elements_(nullptr) {
  if (other.current_size_ != 0) {
    Reserve(other.size());
    AddNAlreadyReserved(other.size());
    CopyArray(Mutable(0), &other.Get(0), other.size());
  }
}

template <typename Element>
template <typename Iter>
RepeatedField<Element>::RepeatedField(Iter begin, const Iter& end)
    : current_size_(0), total_size_(0), arena_or_elements_(nullptr) {
  Add(begin, end);
}

template <typename Element>
RepeatedField<Element>::~RepeatedField() {
#ifndef NDEBUG
  // Try to trigger segfault / asan failure in non-opt builds. If arena_
  // lifetime has ended before the destructor.
  auto arena = GetArena();
  if (arena) (void)arena->SpaceAllocated();
#endif
  if (total_size_ > 0) {
    InternalDeallocate(rep(), total_size_);
  }
}

template <typename Element>
inline void RepeatedField<Element>::Resize(int new_size, const Element& value) {
  GOOGLE_DCHECK_GE(new_size, 0);
  if (new_size > current_size_) {
    Reserve(new_size);
    std::fill(&elements()[current_size_], &elements()[new_size], value);
  }
  current_size_ = new_size;
}

template <typename Element>
inline void RepeatedField<Element>::Add(const Element& value) {
  // https://github.com/protocolbuffers/protobuf/pull/7630/files
  uint32 size = current_size_;
  if (static_cast<int>(size) == total_size_) {
    // value could reference an element of the array. Reserving new space will
    // invalidate the reference. So we must make a copy first.
    auto tmp = value;
    Reserve(total_size_ + 1); // new 一块大小为 total_size_ + 1 的heap，回收原来 total_size_ 的堆。
    elements()[size] = std::move(tmp); // size 起到下标的用途
  } else {
    elements()[size] = value; // 还是以数组的方式来添加的元素的
  }
  current_size_ = size + 1; // 最后， increase current_size_ by 1. 所以 current_size_ 跟 total_size 一样，也是表示元素的个数，starts from 1.
}
/* So the workflow of the Add as below:

* 1. Init State: 
  current_size_==0, total_size_ ==0

* 2. Add one
  current_size_== 1, total_size_ == 1
    current_size_
    |
    V
   [0]
    ^
  total_size_

* 3. Init State after calling Reserve(new_size=n): current_size_ == 0, total_size_ == n(new_size):
    current_size_
     |
     V
    [0][1][...][n-1]
                ^
                |
              total_size_

* 4. Full case:
  when total_size_ == current_size_, new heap and increase total_size_ by 1.
            current_size_
              |
              V
  [0][1][...][n-1]
              ^
              |
            total_size_

* 5. Add one:
  Recall Reserve(new_size = total_size_+1), which set the total_size_ to n(new_size).
           current_size_
             |
             V
[0][1][...][n-1][n]
                 ^
                 |
               total_size_
  At the end of the Add() function, the current_size will be increased by 1 too. At that time, they have the same value: n.

                  current_size_
                  |
                  V
  [0][1][...][n-1][n]
                  ^
                  |
                total_size_
*/

template <typename Element>
template <typename Iter>
inline void RepeatedField<Element>::Add(Iter begin, Iter end) {
  // int reserve = internal::CalculateReserve(begin, end); 
  // equals to std::distance
  int reserve = std::distance(begin, end);
  if (reserve != -1) {
    if (reserve == 0) {
      return;
    }

    Reserve(reserve + size());
    // TODO(ckennelly):  The compiler loses track of the buffer freshly
    // allocated by Reserve() by the time we call elements, so it cannot
    // guarantee that elements does not alias [begin(), end()).
    //
    // If restrict is available, annotating the pointer obtained from elements()
    // causes this to lower to memcpy instead of memmove.
    std::copy(begin, end, elements() + size());
    current_size_ = reserve + size();
  } else {
    FastAdder fast_adder(this);
    for (; begin != end; ++begin) fast_adder.Add(*begin);
  }
}

template <typename Element>
inline size_t RepeatedField<Element>::SpaceUsedExcludingSelfLong() const {
  return total_size_ > 0 ? (total_size_ * sizeof(Element) + kRepHeaderSize) : 0;
}

// Avoid inlining of Reserve(): new, copy, and delete[] lead to a significant
// amount of code bloat.
template <typename Element>
void RepeatedField<Element>::Reserve(int new_size) {
  if (total_size_ >= new_size) return; // 如果新添加的 size 还在范围内，那就直接返回了. 否着分配一个新的可以容纳全部数据的空间

  Rep* old_rep = total_size_ > 0 ? rep() : NULL;
  Rep* new_rep;
  Arena* arena = GetArena();
  new_size = internal::CalculateReserveSize(total_size_, new_size);
  GOOGLE_DCHECK_LE(
      static_cast<size_t>(new_size),
      (std::numeric_limits<size_t>::max() - kRepHeaderSize) / sizeof(Element))
      << "Requested size is too large to fit into size_t.";
  size_t bytes =
      kRepHeaderSize + sizeof(Element) * static_cast<size_t>(new_size);
  if (arena == NULL) {
    new_rep = static_cast<Rep*>(::operator new(bytes)); // 分配一块新的内存区域
  } else {
    new_rep = reinterpret_cast<Rep*>(Arena::CreateArray<char>(arena, bytes));
  }
  new_rep->arena = arena;
  int old_total_size = total_size_;
  // Already known: new_size >= internal::kMinRepeatedFieldAllocationSize
  // Maintain invariant:
  //     total_size_ == 0 ||
  //     total_size_ >= internal::kMinRepeatedFieldAllocationSize
  total_size_ = new_size;
  arena_or_elements_ = new_rep->elements;
  // Invoke placement-new on newly allocated elements. We shouldn't have to do
  // this, since Element is supposed to be POD, but a previous version of this
  // code allocated storage with "new Element[size]" and some code uses
  // RepeatedField with non-POD types, relying on constructor invocation. If
  // Element has a trivial constructor (e.g., int32), gcc (tested with -O2)
  // completely removes this loop because the loop body is empty, so this has no
  // effect unless its side-effects are required for correctness.
  // Note that we do this before MoveArray() below because Element's copy
  // assignment implementation will want an initialized instance first.
  Element* e = &elements()[0];
  Element* limit = e + total_size_;
  for (; e < limit; e++) {
    new (e) Element; // Calling placement-new
  }
  if (current_size_ > 0) {
    MoveArray(&elements()[0], old_rep->elements, current_size_);
  }

  // Likewise, we need to invoke destructors on the old array.
  InternalDeallocate(old_rep, old_total_size);
}

template <typename Element>
inline void RepeatedField<Element>::Truncate(int new_size) {
  GOOGLE_DCHECK_LE(new_size, current_size_);
  if (current_size_ > 0) {
    current_size_ = new_size;
  }
}

template <typename Element>
inline void RepeatedField<Element>::MoveArray(Element* to, Element* from,
                                              int array_size) {
  CopyArray(to, from, array_size);
}

template <typename Element>
inline void RepeatedField<Element>::CopyArray(Element* to, const Element* from,
                                              int array_size) {
  internal::ElementCopier<Element>()(to, from, array_size);
}

namespace internal {
 // Returns the new size for a reserved field based on its 'total_size' and the
// requested 'new_size'. The result is clamped to the closed interval:
//   [internal::kMinRepeatedFieldAllocationSize,
//    std::numeric_limits<int>::max()]
// Requires:
//     new_size > total_size &&
//     (total_size == 0 ||
//      total_size >= kRepeatedFieldLowerClampLimit)
inline int CalculateReserveSize(int total_size, int new_size) {
  if (new_size < kRepeatedFieldLowerClampLimit) {
    // Clamp to smallest allowed size.
    return kRepeatedFieldLowerClampLimit;
  }
  if (total_size < kRepeatedFieldUpperClampLimit) {
    return std::max(total_size * 2, new_size);
  } else {
    // Clamp to largest allowed size.
    GOOGLE_DCHECK_GT(new_size, kRepeatedFieldUpperClampLimit);
    return std::numeric_limits<int>::max();
  }
}


// This is a helper template to copy an array of elements efficiently when they
// have a trivial copy constructor, and correctly otherwise. This really
// shouldn't be necessary, but our compiler doesn't optimize std::copy very
// effectively.
template <typename Element,
          bool HasTrivialCopy = std::is_standard_layout<Element>::value && std::is_trivial<Element>::value>
struct ElementCopier {
  void operator()(Element* to, const Element* from, int array_size);
};

template <typename Element, bool HasTrivialCopy>
void ElementCopier<Element, HasTrivialCopy>::operator()(Element* to, const Element* from, int array_size) {
  std::copy(from, from + array_size, to); // Copy elements by traversing. Low efficient
}

template <typename Element>
struct ElementCopier<Element, true> { // specialize class template. For standar_layout and trivial element, it use memcpy.
  void operator()(Element* to, const Element* from, int array_size) {
    memcpy(to, from, static_cast<size_t>(array_size) * sizeof(Element));
  }
};
```

- src/google/protobuf/repeated_field.cc
Instantial the template. To reduce caller's compile time.

```c++
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<bool>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<int32>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<uint32>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<int64>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<uint64>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<float>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<double>;
```

- Summary
  - Holds a successive heap space, underlying type is byte array.


## [What datatype should I use for a small int values in protobuffer?](https://stackoverflow.com/questions/50463770/what-datatype-should-i-use-for-a-small-int-values-in-protobuffer)

- Q: 
  I looked at scalar value types and it seems there's no option to store small int values (practically I need integer values 1 through 10). Is there any option other than creating a enum with 10 possible values for this?

- A: 
  You are right in that there is no specific support for a smaller primitive; that's because the schema syntax needs to support a wide range of implementations on different platforms and languages, so the types are intentionally left to a subset that will be available for most systems.

  Enumerations (enum in .proto) are treated as int32 values, using "varint" encoding. There's no advantage between using int32 vs declaring an enum with values Zero, One, etc. So you might as well just use int32. As long as you only use low values, it will automatically encode very efficiently, simply because of how "varint" encoding works - anything in the range 0-127 will take a single byte to encode on the wire (plus the field header, which is usually another single byte for simple values).

- Other useful links
  - https://protobuf.dev/programming-guides/encoding/

# Varint and ZigZag
- https://stackoverflow.com/questions/24614553/why-is-varint-an-efficient-data-representation

- https://zhuanlan.zhihu.com/p/542208233?utm_id=0
- https://zhuanlan.zhihu.com/p/561305473
- https://blog.csdn.net/weixin_44477424/article/details/131796087
- https://www.codenong.com/j5dfe0cd5518825124c5/
- https://www.jianshu.com/p/074359bea057?utm_campaign=hugo&utm_content=note&utm_medium=writer_share&utm_source=weibo
- https://blog.csdn.net/qq_51492202/article/details/131128939

- src/google/protobuf/stubs/casts.h
```c++
template<typename To, typename From>
inline To bit_cast(const From& from) {
  GOOGLE_COMPILE_ASSERT(sizeof(From) == sizeof(To),
                        bit_cast_with_different_sizes);
  To dest;
  memcpy(&dest, &from, sizeof(dest));
  return dest;
}
```
- src/google/protobuf/wire_format_lite.h
```c++
  // Helper functions for converting between floats/doubles and IEEE-754
  // uint32s/uint64s so that they can be written.  (Assumes your platform
  // uses IEEE-754 floats.)
  static uint32 EncodeFloat(float value);
  static float DecodeFloat(uint32 value);
  static uint64 EncodeDouble(double value);
  static double DecodeDouble(uint64 value);

  // Helper functions for mapping signed integers to unsigned integers in
  // such a way that numbers with small magnitudes will encode to smaller
  // varints.  If you simply static_cast a negative number to an unsigned
  // number and varint-encode it, it will always take 10 bytes, defeating
  // the purpose of varint.  So, for the "sint32" and "sint64" field types,
  // we ZigZag-encode the values.
  static uint32 ZigZagEncode32(int32 n);
  static int32 ZigZagDecode32(uint32 n);
  static uint64 ZigZagEncode64(int64 n);
  static int64 ZigZagDecode64(uint64 n);

inline uint32 WireFormatLite::EncodeFloat(float value) {
  return bit_cast<uint32>(value);
}

inline float WireFormatLite::DecodeFloat(uint32 value) {
  return bit_cast<float>(value);
}

inline uint64 WireFormatLite::EncodeDouble(double value) {
  return bit_cast<uint64>(value);
}

inline double WireFormatLite::DecodeDouble(uint64 value) {
  return bit_cast<double>(value);
}

// ZigZag Transform:  Encodes signed integers so that they can be
// effectively used with varint encoding.
//
// varint operates on unsigned integers, encoding smaller numbers into
// fewer bytes.  If you try to use it on a signed integer, it will treat
// this number as a very large unsigned integer, which means that even
// small signed numbers like -1 will take the maximum number of bytes
// (10) to encode.  ZigZagEncode() maps signed integers to unsigned
// in such a way that those with a small absolute value will have smaller
// encoded values, making them appropriate for encoding using varint.
//
//       int32 ->     uint32
// -------------------------
//           0 ->          0
//          -1 ->          1
//           1 ->          2
//          -2 ->          3
//         ... ->        ...
//  2147483647 -> 4294967294
// -2147483648 -> 4294967295
//
//        >> encode >>
//        << decode <<

inline uint32 WireFormatLite::ZigZagEncode32(int32 n) {
  // Note:  the right-shift must be arithmetic
  // Note:  left shift must be unsigned because of overflow
  return (static_cast<uint32>(n) << 1) ^ static_cast<uint32>(n >> 31);
}

inline int32 WireFormatLite::ZigZagDecode32(uint32 n) {
  // Note:  Using unsigned types prevent undefined behavior
  return static_cast<int32>((n >> 1) ^ (~(n & 1) + 1));
}

inline uint64 WireFormatLite::ZigZagEncode64(int64 n) {
  // Note:  the right-shift must be arithmetic
  // Note:  left shift must be unsigned because of overflow
  return (static_cast<uint64>(n) << 1) ^ static_cast<uint64>(n >> 63);
}

inline int64 WireFormatLite::ZigZagDecode64(uint64 n) {
  // Note:  Using unsigned types prevent undefined behavior
  return static_cast<int64>((n >> 1) ^ (~(n & 1) + 1));
}
```
- src/google/protobuf/io/coded_stream.h

```c++
  template <typename T, typename E>
  PROTOBUF_ALWAYS_INLINE uint8* WriteVarintPacked(int num, const T& r, int size,
                                                  uint8* ptr, const E& encode) {
    ptr = EnsureSpace(ptr);
    ptr = WriteLengthDelim(num, size, ptr);
    auto it = r.data();
    auto end = it + r.size();
    do {
      ptr = EnsureSpace(ptr);
      ptr = UnsafeVarint(encode(*it++), ptr);
    } while (it < end);
    return ptr;
  }

  static uint32 Encode32(uint32 v) { return v; }
  static uint64 Encode64(uint64 v) { return v; }

  static uint32 ZigZagEncode32(int32 v) {
    return (static_cast<uint32>(v) << 1) ^ static_cast<uint32>(v >> 31);
  }
  static uint64 ZigZagEncode64(int64 v) {
    return (static_cast<uint64>(v) << 1) ^ static_cast<uint64>(v >> 63);
  }

  template <typename T>
  PROTOBUF_ALWAYS_INLINE uint8* WriteInt32Packed(int num, const T& r, int size, uint8* ptr) {
    return WriteVarintPacked(num, r, size, ptr, Encode64); // Int32 调用普通编码方式
  }

  template <typename T>
  PROTOBUF_ALWAYS_INLINE uint8* WriteUInt32Packed(int num, const T& r, int size, uint8* ptr) {
    return WriteVarintPacked(num, r, size, ptr, Encode32); // UInt32 也是调用普通编码方式
  }
  
  template <typename T>
  PROTOBUF_ALWAYS_INLINE uint8* WriteSInt32Packed(int num, const T& r, int size, uint8* ptr) {
    return WriteVarintPacked(num, r, size, ptr, ZigZagEncode32); // 只有 SInt32 才会调用 ZigZag 方式
  }
```

- src/google/protobuf/descriptor.cc
```c++
// Convenience functions to set an int field the right way, depending on
// its wire type (a single int CppType can represent multiple wire types).
void SetInt32(int number, int32 value, FieldDescriptor::Type type, UnknownFieldSet* unknown_fields);

void DescriptorBuilder::OptionInterpreter::SetInt32(int number, int32 value, FieldDescriptor::Type type, UnknownFieldSet* unknown_fields) {
  switch (type) {
    case FieldDescriptor::TYPE_INT32:
      unknown_fields->AddVarint(number, static_cast<uint64>(static_cast<int64>(value))); //  int32 -> int64 -> uint64
      break;

    case FieldDescriptor::TYPE_SFIXED32:
      unknown_fields->AddFixed32(number, static_cast<uint32>(value)); // fixed int32 to uint32
      break;

    case FieldDescriptor::TYPE_SINT32:
      unknown_fields->AddVarint(number, internal::WireFormatLite::ZigZagEncode32(value)); // Apply the ZigZag for sint32
      break;

    default:
      GOOGLE_LOG(FATAL) << "Invalid wire type for CPPTYPE_INT32: " << type;
      break;
  }

  void DescriptorBuilder::OptionInterpreter::SetUInt32( int number, uint32 value, FieldDescriptor::Type type, UnknownFieldSet* unknown_fields) {
  switch (type) {
    case FieldDescriptor::TYPE_UINT32:
      unknown_fields->AddVarint(number, static_cast<uint64>(value));
      break;

    case FieldDescriptor::TYPE_FIXED32:
      unknown_fields->AddFixed32(number, static_cast<uint32>(value));
      break;

    default:
      GOOGLE_LOG(FATAL) << "Invalid wire type for CPPTYPE_UINT32: " << type;
      break;
  }
}

    // Validates the value for the option field of the currently interpreted
    // option and then sets it on the unknown_field.
    bool SetOptionValue(const FieldDescriptor* option_field, UnknownFieldSet* unknown_fields);
    
bool DescriptorBuilder::OptionInterpreter::SetOptionValue(
    const FieldDescriptor* option_field, UnknownFieldSet* unknown_fields) {
  // We switch on the CppType to validate.
  switch (option_field->cpp_type()) {
    case FieldDescriptor::CPPTYPE_INT32:
      if (uninterpreted_option_->has_positive_int_value()) {
        if (uninterpreted_option_->positive_int_value() >
            static_cast<uint64>(kint32max)) {
          return AddValueError("Value out of range for int32 option \"" +
                               option_field->full_name() + "\".");
        } else {
          SetInt32(option_field->number(),
                   uninterpreted_option_->positive_int_value(),
                   option_field->type(), unknown_fields);
        }
      } else if (uninterpreted_option_->has_negative_int_value()) {
        if (uninterpreted_option_->negative_int_value() <
            static_cast<int64>(kint32min)) {
          return AddValueError("Value out of range for int32 option \"" +
                               option_field->full_name() + "\".");
        } else {
          SetInt32(option_field->number(),
                   uninterpreted_option_->negative_int_value(),
                   option_field->type(), unknown_fields);
        }
      } else {
        return AddValueError("Value must be integer for int32 option \"" +
                             option_field->full_name() + "\".");
      }
      break;
      ...
  }
}
```

- src/google/protobuf/descriptor.h
```c++
  // Specifies the C++ data type used to represent the field.  There is a
  // fixed mapping from Type to CppType where each Type maps to exactly one
  // CppType.  0 is reserved for errors.
  enum CppType {
    CPPTYPE_INT32 = 1,     // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
    CPPTYPE_INT64 = 2,     // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
    CPPTYPE_UINT32 = 3,    // TYPE_UINT32, TYPE_FIXED32
    CPPTYPE_UINT64 = 4,    // TYPE_UINT64, TYPE_FIXED64
    CPPTYPE_DOUBLE = 5,    // TYPE_DOUBLE
    CPPTYPE_FLOAT = 6,     // TYPE_FLOAT
    CPPTYPE_BOOL = 7,      // TYPE_BOOL
    CPPTYPE_ENUM = 8,      // TYPE_ENUM
    CPPTYPE_STRING = 9,    // TYPE_STRING, TYPE_BYTES
    CPPTYPE_MESSAGE = 10,  // TYPE_MESSAGE, TYPE_GROUP

    MAX_CPPTYPE = 10,  // Constant useful for defining lookup tables
                       // indexed by CppType.
  };
```

- https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/type.proto
```
// A single field of a message type.
message Field {
  // Basic field types.
  enum Kind {
    // Field type unknown.
    TYPE_UNKNOWN = 0;
    // Field type double.
    TYPE_DOUBLE = 1;
    // Field type float.
    TYPE_FLOAT = 2;
    // Field type int64.
    TYPE_INT64 = 3;
    // Field type uint64.
    TYPE_UINT64 = 4;
    // Field type int32.
    TYPE_INT32 = 5;
    // Field type fixed64.
    TYPE_FIXED64 = 6;
    // Field type fixed32.
    TYPE_FIXED32 = 7;
    // Field type bool.
    TYPE_BOOL = 8;
    // Field type string.
    TYPE_STRING = 9;
    // Field type group. Proto2 syntax only, and deprecated.
    TYPE_GROUP = 10;
    // Field type message.
    TYPE_MESSAGE = 11;
    // Field type bytes.
    TYPE_BYTES = 12;
    // Field type uint32.
    TYPE_UINT32 = 13;
    // Field type enum.
    TYPE_ENUM = 14;
    // Field type sfixed32.
    TYPE_SFIXED32 = 15;
    // Field type sfixed64.
    TYPE_SFIXED64 = 16;
    // Field type sint32.
    TYPE_SINT32 = 17;
    // Field type sint64.
    TYPE_SINT64 = 18;
  }
}
```

# SQLite 
- https://www.sqlite.org/fileformat.html
- https://www.sqlite.org/fileformat2.html#varint

# Arena

# coded_stream.h
- 

Author: kenton@google.com (Kenton Varda) Based on original Protocol Buffers design by Sanjay Ghemawat, Jeff Dean, and others.

This file contains the CodedInputStream and CodedOutputStream classes,
which wrap a ZeroCopyInputStream or ZeroCopyOutputStream, respectively,
and allow you to read or write individual pieces of data in various
formats.  In particular, these implement the varint encoding for
integers, a simple variable-length encoding in which smaller numbers
take fewer bytes.

Typically these classes will only be used internally by the protocol
buffer library in order to encode and decode protocol buffers.  Clients
of the library only need to know about this class if they wish to write
custom message parsing or serialization procedures.


- CodedOutputStream example:
```c++
   // Write some data to "myfile".  First we write a 4-byte "magic number"
   // to identify the file type, then write a length-delimited string.  The
   // string is composed of a varint giving the length followed by the raw
   // bytes.
   int fd = open("myfile", O_CREAT | O_WRONLY);
   ZeroCopyOutputStream* raw_output = new FileOutputStream(fd);
   CodedOutputStream* coded_output = new CodedOutputStream(raw_output);

   int magic_number = 1234;
   char text[] = "Hello world!";
   coded_output->WriteLittleEndian32(magic_number);
   coded_output->WriteVarint32(strlen(text));
   coded_output->WriteRaw(text, strlen(text));

   delete coded_output;
   delete raw_output;
   close(fd);
```

- CodedInputStream example:
```c++
   // Read a file created by the above code.
   int fd = open("myfile", O_RDONLY);
   ZeroCopyInputStream* raw_input = new FileInputStream(fd);
   CodedInputStream* coded_input = new CodedInputStream(raw_input);

   coded_input->ReadLittleEndian32(&magic_number);
   if (magic_number != 1234) {
     cerr << "File not in expected format." << endl;
     return;
   }

   uint32 size;
   coded_input->ReadVarint32(&size);

   char* text = new char[size + 1];
   coded_input->ReadRaw(text, size);
   text[size] = '\0';

   delete coded_input;
   delete raw_input;
   close(fd);

   cout << "Text is: " << text << endl;
   delete [] text;
```

For those who are interested, varint encoding is defined as follows:

The encoding operates on unsigned integers of up to 64 bits in length.
Each byte of the encoded value has the format:
* bits 0-6: Seven bits of the number being encoded.
* bit 7: Zero if this is the last byte in the encoding (in which
  case all remaining bits of the number are zero) or 1 if
  more bytes follow.

The first byte contains the least-significant 7 bits of the number, the
second byte (if present) contains the next-least-significant 7 bits,
and so on.  So, the binary number 1011000101011 would be encoded in two
bytes as "10101011 00101100".

In theory, varint could be used to encode integers of any length.
However, for practicality we set a limit at 64 bits.  The maximum encoded
length of a number is thus 10 bytes.

### CodedInputStream
- ctor
```c++
  // Create a CodedInputStream that reads from the given ZeroCopyInputStream.
  explicit CodedInputStream(ZeroCopyInputStream* input);

  // Create a CodedInputStream that reads from the given flat array.  This is
  // faster than using an ArrayInputStream.  PushLimit(size) is implied by
  // this constructor.
  explicit CodedInputStream(const uint8* buffer, int size);
```

- dtor
```c++
  // Destroy the CodedInputStream and position the underlying
  // ZeroCopyInputStream at the first unread byte.  If an error occurred while
  // reading (causing a method to return false), then the exact position of
  // the input stream may be anywhere between the last value that was read
  // successfully and the stream's byte limit.
  ~CodedInputStream();
```

- Reads
```c++
// Read raw bytes, copying them into the given buffer.
  bool ReadRaw(void* buffer, int size);

  // Like ReadRaw, but reads into a string.
  bool ReadString(std::string* buffer, int size);

  // Read a 32-bit little-endian integer.
  bool ReadLittleEndian32(uint32* value);
  // Read a 64-bit little-endian integer.
  bool ReadLittleEndian64(uint64* value);

  // These methods read from an externally provided buffer. The caller is
  // responsible for ensuring that the buffer has sufficient space.
  // Read a 32-bit little-endian integer.
  static const uint8* ReadLittleEndian32FromArray(const uint8* buffer, uint32* value);
  // Read a 64-bit little-endian integer.
  static const uint8* ReadLittleEndian64FromArray(const uint8* buffer, uint64* value);

  // Read an unsigned integer with Varint encoding, truncating to 32 bits.
  // Reading a 32-bit value is equivalent to reading a 64-bit one and casting
  // it to uint32, but may be more efficient.
  bool ReadVarint32(uint32* value);
  // Read an unsigned integer with Varint encoding.
  bool ReadVarint64(uint64* value);

  // Reads a varint off the wire into an "int". This should be used for reading
  // sizes off the wire (sizes of strings, submessages, bytes fields, etc).
  //
  // The value from the wire is interpreted as unsigned.  If its value exceeds
  // the representable value of an integer on this platform, instead of
  // truncating we return false. Truncating (as performed by ReadVarint32()
  // above) is an acceptable approach for fields representing an integer, but
  // when we are parsing a size from the wire, truncating the value would result
  // in us misparsing the payload.
  bool ReadVarintSizeAsInt(int* value);
```

- Tag

- Recursion Limit
```c++
  // Recursion Limit -------------------------------------------------
  // To prevent corrupt or malicious messages from causing stack overflows,
  // we must keep track of the depth of recursion when parsing embedded
  // messages and groups.  CodedInputStream keeps track of this because it
  // is the only object that is passed down the stack during parsing.

  // Sets the maximum recursion depth.  The default is 100.
  void SetRecursionLimit(int limit);
  int RecursionBudget() { return recursion_budget_; }

  static int GetDefaultRecursionLimit() { return default_recursion_limit_; }
  // Increments the current recursion depth.  Returns true if the depth is
  // under the limit, false if it has gone over.
  bool IncrementRecursionDepth();

  // Decrements the recursion depth if possible.
  void DecrementRecursionDepth();

```
#### Details
```c++
inline void CodedInputStream::Advance(int amount) { buffer_ += amount; }

bool CodedInputStream::ReadVarint32Slow(uint32* value) {
  // Directly invoke ReadVarint64Fallback, since we already tried to optimize
  // for one-byte varints.
  std::pair<uint64, bool> p = ReadVarint64Fallback();
  *value = static_cast<uint32>(p.first);
  return p.second;
}

int64 CodedInputStream::ReadVarint32Fallback(uint32 first_byte_or_zero) {
  if (BufferSize() >= kMaxVarintBytes ||
      // Optimization:  We're also safe if the buffer is non-empty and it ends with a byte that would terminate a varint.
      (buffer_end_ > buffer_ && !(buffer_end_[-1] & 0x80))) {
    GOOGLE_DCHECK_NE(first_byte_or_zero, 0) << "Caller should provide us with *buffer_ when buffer is non-empty";

    uint32 temp;
    std::pair<bool, const uint8*> p = ReadVarint32FromArray(first_byte_or_zero, buffer_, &temp);
    if (!p.first) return -1; // 失败时，返回 -1
    buffer_ = p.second;
    return temp;
  } else {
    // Really slow case: we will incur the cost of an extra function call here,
    // but moving this out of line reduces the size of this function, which
    // improves the common case. In micro benchmarks, this is worth about 10-15%
    uint32 temp;
    return ReadVarint32Slow(&temp) ? static_cast<int64>(temp) : -1;
  }
}

// inline methods ====================================================
// The vast majority of varints are only one byte.
// These inline methods optimize for that case.
inline bool CodedInputStream::ReadVarint32(uint32* value) {
  uint32 v = 0;
  if (PROTOBUF_PREDICT_TRUE(buffer_ < buffer_end_)) {
    v = *buffer_; // 这里是赋值，即把 buffer_ 的第一个字节赋值给 v.
    if (v < 0x80) { // true 的条件是 v 的最高位是0，表示这是一个 uint8 是个独立的单元.
      *value = v;
      Advance(1); // 使 buffer_ 指向下一个有效 byte.
      return true;
    }
  }
  int64 result = ReadVarint32Fallback(v); // v是 *buffer_, 单位 unit8, 即 first_byte 
  *value = static_cast<uint32>(result);
  return result >= 0; // result=-1 when failed to decode it.
}

static const int kMaxVarintBytes = 10;
static const int kMaxVarint32Bytes = 5;

// Read a varint from the given buffer, write it to *value, and return a pair.
// The first part of the pair is true if the read was successful.
// The second part is buffer + (number of bytes read).
// This function is always inlined, so returning a pair is costless. 这句话怎么理解？
PROTOBUF_ALWAYS_INLINE
std::pair<bool, const uint8*> ReadVarint32FromArray(uint32 first_byte, const uint8* buffer, uint32* value) {
    // Fast path: 
    // We have enough bytes left in the buffer to guarantee that this read won't cross the end, so we can skip the checks.
    GOOGLE_DCHECK_EQ(*buffer, first_byte);
    GOOGLE_DCHECK_EQ(first_byte & 0x80, 0x80) << first_byte;
    /* After `const uint8* ptr = buffer;`
       buffer
         v
        [ ][ ][ ]...[ ][ ]
         ^
        ptr
    */
    const uint8* ptr = buffer;
    uint32 b;
    uint32 result = first_byte - 0x80; // 这里的 first_byte 必然大于 0x80, 所以，减法不会溢出，result 就是 Least-Significant 7 bits.
    /* After `++ptr;`
       buffer
         v
        [ ][ ][ ]...[ ][ ]
            ^
           ptr
    */
    ++ptr;  // We just processed the first byte. Move on to the second.
    /* After `b = *(ptr++);`
       buffer
         v
        [ ][ ][ ]...[ ][ ]
            ^  ^
            | ptr
            |
        it equals to b.
    */
    b = *(ptr++); // 这是第几个字节？ 第2个byte, b是ptr加1前的值。等价于 `b = *ptr; ptr=ptr+1;`
    result += b << 7;
    if (!(b & 0x80)) goto done; // 即 b 的最高位是0, 表示该字节是当前的数的最后一个组成部分，完成int32数值的组装。
    // 否则，记录处理第三个字节
    result -= 0x80 << 7; // 这里为什么要减去 0x80<<7('0x4000'）？ 因为if前的 `result += b << 7;` 有把b（第2个byte）的最高位加进去，但这个位只是个标志位。所以这里需要减掉.
    /* After `b = *(ptr++);`
       buffer
         v
        [0][1][2][3]...[ ][ ]
               ^  ^
               | ptr
               |
           it equals to b.
    */
    b = *(ptr++);
    result += b << 14;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 14; // 减掉b（第3个byte）的标志位
    /* After `b = *(ptr++);`
       buffer
         v
        [0][1][2][3][4]...[ ][ ]
                  ^  ^
                  | ptr
                  |
              it equals to b.
    */
    b = *(ptr++);
    result += b << 21;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 21; // 减掉b（第4个byte）的标志位
    /* After `b = *(ptr++);`
       buffer
         v
        [0][1][2][3][4][5]...[ ][ ]
                     ^  ^
                     | ptr
                     |
                 it equals to b.
    */
    b = *(ptr++);
    result += b << 28;
    if (!(b & 0x80)) goto done; // 就是说，它有可能需要5个字节来存放一个 int32.
    // "result -= 0x80 << 28" is irrevelant.

    // If the input is larger than 32 bits, we still need to read it all
    // and discard the high-order bits. 虽然继续处理，但已经不更新 ptr 了
    for (int i = 0; i < kMaxVarintBytes - kMaxVarint32Bytes; i++) {
        b = *(ptr++);
        if (!(b & 0x80)) goto done;
    }

    // We have overrun the maximum size of a varint (10 bytes).  Assume the data is corrupt.
    return std::make_pair(false, ptr);

done:
    *value = result;
    return std::make_pair(true, ptr);
}
```

### CodedOutputStream

```c++
   CodedOutputStream* coded_output = new CodedOutputStream(raw_output);
   int magic_number = 1234;
   char text[] = "Hello world!";

   int coded_size = sizeof(magic_number) +
                    CodedOutputStream::VarintSize32(strlen(text)) +
                    strlen(text);

   uint8* buffer =
       coded_output->GetDirectBufferForNBytesAndAdvance(coded_size);
   if (buffer != nullptr) {
     // The output stream has enough space in the buffer: write directly to
     // the array.
     buffer = CodedOutputStream::WriteLittleEndian32ToArray(magic_number,
                                                            buffer);
     buffer = CodedOutputStream::WriteVarint32ToArray(strlen(text), buffer);
     buffer = CodedOutputStream::WriteRawToArray(text, strlen(text), buffer);
   } else {
     // Make bound-checked writes, which will ask the underlying stream for
     // more space as needed.
     coded_output->WriteLittleEndian32(magic_number);
     coded_output->WriteVarint32(strlen(text));
     coded_output->WriteRaw(text, strlen(text));
   }
```
- ctor
```c++
// Create an CodedOutputStream that writes to the given ZeroCopyOutputStream.
  explicit CodedOutputStream(ZeroCopyOutputStream* stream)
      : CodedOutputStream(stream, true) {}

  CodedOutputStream(ZeroCopyOutputStream* stream, bool do_eager_refresh);
```
- dtor
```c++
  // Destroy the CodedOutputStream and position the underlying
  // ZeroCopyOutputStream immediately after the last byte written.
  ~CodedOutputStream();
```

- Write
```c++
  // Write raw bytes, copying them from the given buffer.
  void WriteRaw(const void* buffer, int size) {
    cur_ = impl_.WriteRaw(buffer, size, cur_);
  }
  // Like WriteRaw()  but will try to write aliased data if aliasing is
  // turned on.
  void WriteRawMaybeAliased(const void* data, int size);
  // Like WriteRaw()  but writing directly to the target array.
  // This is _not_ inlined, as the compiler often optimizes memcpy into inline
  // copy loops. Since this gets called by every field with string or bytes
  // type, inlining may lead to a significant amount of code bloat, with only a
  // minor performance gain.
  static uint8* WriteRawToArray(const void* buffer, int size, uint8* target);

  // Equivalent to WriteRaw(str.data(), str.size()).
  void WriteString(const std::string& str);
  // Like WriteString()  but writing directly to the target array.
  static uint8* WriteStringToArray(const std::string& str, uint8* target);

  // Write the varint-encoded size of str followed by str.
  static uint8* WriteStringWithSizeToArray(const std::string& str, uint8* target);

  // Write a 32-bit little-endian integer.
  void WriteLittleEndian32(uint32 value) {
    cur_ = impl_.EnsureSpace(cur_);
    SetCur(WriteLittleEndian32ToArray(value, Cur()));
  }
  // Like WriteLittleEndian32()  but writing directly to the target array.
  static uint8* WriteLittleEndian32ToArray(uint32 value, uint8* target);
  // Write a 64-bit little-endian integer.
  void WriteLittleEndian64(uint64 value) {
    cur_ = impl_.EnsureSpace(cur_);
    SetCur(WriteLittleEndian64ToArray(value, Cur()));
  }
  // Like WriteLittleEndian64()  but writing directly to the target array.
  static uint8* WriteLittleEndian64ToArray(uint64 value, uint8* target);

  // Write an unsigned integer with Varint encoding.  Writing a 32-bit value
  // is equivalent to casting it to uint64 and writing it as a 64-bit value,
  // but may be more efficient.
  void WriteVarint32(uint32 value);
  // Like WriteVarint32()  but writing directly to the target array.
  static uint8* WriteVarint32ToArray(uint32 value, uint8* target);
  // Write an unsigned integer with Varint encoding.
  void WriteVarint64(uint64 value);
  // Like WriteVarint64()  but writing directly to the target array.
  static uint8* WriteVarint64ToArray(uint64 value, uint8* target);

  // Equivalent to WriteVarint32() except when the value is negative,
  // in which case it must be sign-extended to a full 10 bytes.
  void WriteVarint32SignExtended(int32 value);
  // Like WriteVarint32SignExtended()  but writing directly to the target array.
  static uint8* WriteVarint32SignExtendedToArray(int32 value, uint8* target);
```

- Write Length
```c++
 // Returns the number of bytes needed to encode the given value as a varint.
  static size_t VarintSize32(uint32 value);
  // Returns the number of bytes needed to encode the given value as a varint.
  static size_t VarintSize64(uint64 value);

  // If negative, 10 bytes.  Otherwise, same as VarintSize32().
  static size_t VarintSize32SignExtended(int32 value);

  // Compile-time equivalent of VarintSize32().
  template <uint32 Value>
  struct StaticVarintSize32 {
    static const size_t value =
        (Value < (1 << 7))
            ? 1
            : (Value < (1 << 14))
                  ? 2
                  : (Value < (1 << 21)) ? 3 : (Value < (1 << 28)) ? 4 : 5;
  };

  // Returns the total number of bytes written since this object was created.
  int ByteCount() const {
    return static_cast<int>(impl_.ByteCount(cur_) - start_count_);
  }
```

- Write Tag
```c++
  // This is identical to WriteVarint32(), but optimized for writing tags.
  // In particular, if the input is a compile-time constant, this method
  // compiles down to a couple instructions.
  // Always inline because otherwise the aforementioned optimization can't work,
  // but GCC by default doesn't want to inline this.
  void WriteTag(uint32 value);
  // Like WriteTag()  but writing directly to the target array.
  PROTOBUF_ALWAYS_INLINE
  static uint8* WriteTagToArray(uint32 value, uint8* target);
```

- Serialize
```c++
  template <typename Func>
  void Serialize(const Func& func);

  uint8* Cur() const { return cur_; }
  
  void SetCur(uint8* ptr) { cur_ = ptr; }
  
  EpsCopyOutputStream* EpsCopy() { return &impl_; }
 private:
  EpsCopyOutputStream impl_;
  uint8* cur_;
  int64 start_count_;
```

### EpsCopyOutputStream
EpsCopyOutputStream wraps a ZeroCopyOutputStream and exposes a new stream,
which has the property you can write kSlopBytes (16 bytes) from the current
position without bounds checks. The cursor into the stream is managed by
the user of the class and is an explicit parameter in the methods. Careful
use of this class, ie. keep ptr a local variable, eliminates the need to
for the compiler to sync the ptr value between register and memory.

```c++

  static constexpr int TagSize(uint32 tag) {
    return (tag < (1 << 7))
               ? 1
               : (tag < (1 << 14))
                     ? 2
                     : (tag < (1 << 21)) ? 3 : (tag < (1 << 28)) ? 4 : 5;
  }

  PROTOBUF_ALWAYS_INLINE uint8* WriteTag(uint32 num, uint32 wt, uint8* ptr) {
    GOOGLE_DCHECK(ptr < end_);  // NOLINT
    return UnsafeVarint((num << 3) | wt, ptr);
  }


uint8* EpsCopyOutputStream::EnsureSpaceFallback(uint8* ptr) {
  do {
    if (PROTOBUF_PREDICT_FALSE(had_error_)) return buffer_;
    int overrun = ptr - end_;
    GOOGLE_DCHECK(overrun >= 0);           // NOLINT
    GOOGLE_DCHECK(overrun <= kSlopBytes);  // NOLINT
    ptr = Next() + overrun;
  } while (ptr >= end_);
  GOOGLE_DCHECK(ptr < end_);  // NOLINT
  return ptr;
}

uint8* EpsCopyOutputStream::WriteRawFallback(const void* data, int size,
                                             uint8* ptr) {
  int s = GetSize(ptr);
  while (s < size) {
    std::memcpy(ptr, data, s);
    size -= s;
    data = static_cast<const uint8*>(data) + s;
    ptr = EnsureSpaceFallback(ptr + s);
    s = GetSize(ptr);
  }
  std::memcpy(ptr, data, size);
  return ptr + size;
}

  // After this it's guaranteed you can safely write kSlopBytes to ptr. This
  // will never fail! The underlying stream can produce an error. Use HadError
  // to check for errors.
  PROTOBUF_MUST_USE_RESULT uint8* EnsureSpace(uint8* ptr) {
    if (PROTOBUF_PREDICT_FALSE(ptr >= end_)) {
      return EnsureSpaceFallback(ptr);
    }
    return ptr;
  }

  uint8* WriteRaw(const void* data, int size, uint8* ptr) {
    if (PROTOBUF_PREDICT_FALSE(end_ - ptr < size)) {
      return WriteRawFallback(data, size, ptr);
    }
    std::memcpy(ptr, data, size);
    return ptr + size;
  }

  template <typename T>
  PROTOBUF_ALWAYS_INLINE uint8* WriteString(uint32 num, const T& s,
                                            uint8* ptr) {
    std::ptrdiff_t size = s.size();
    if (PROTOBUF_PREDICT_FALSE(
            size >= 128 || end_ - ptr + 16 - TagSize(num << 3) - 1 < size)) {
      return WriteStringOutline(num, s, ptr);
    }
    ptr = UnsafeVarint((num << 3) | 2, ptr);
    *ptr++ = static_cast<uint8>(size);
    std::memcpy(ptr, s.data(), size);
    return ptr + size;
  }
  static constexpr int TagSize(uint32 tag) {
    return (tag < (1 << 7))
               ? 1
               : (tag < (1 << 14))
                     ? 2
                     : (tag < (1 << 21)) ? 3 : (tag < (1 << 28)) ? 4 : 5;
  }

  template <typename T, typename E>
  PROTOBUF_ALWAYS_INLINE uint8* WriteVarintPacked(int num, const T& r, int size,
                                                  uint8* ptr, const E& encode) {
    ptr = EnsureSpace(ptr);
    ptr = WriteLengthDelim(num, size, ptr);
    auto it = r.data();
    auto end = it + r.size();
    do {
      ptr = EnsureSpace(ptr);
      ptr = UnsafeVarint(encode(*it++), ptr);
    } while (it < end);
    return ptr;
  }

template <typename T>
PROTOBUF_ALWAYS_INLINE static uint8* UnsafeVarint(T value, uint8* ptr) {
    // 只能是无符号整形
    static_assert(std::is_unsigned<T>::value, "Varint serialization must be unsigned");
    if (value < 0x80) {  // 一个小于 128 的数字
        ptr[0] = static_cast<uint8>(value);
        return ptr + 1;
    } else {                                        // 处理大于 128 的数字
        ptr[0] = static_cast<uint8>(value | 0x80);  // 保留低7位, 并将第8位设置为1
        // >>> bin(0x01|0x80)
        // '0b10000001'
        // >>> bin(0x91|0x80)
        // '0b10010001'

        value >>= 7;  // value为去掉LSB 7bit后的
        if (value < 0x80) {
            ptr[1] = static_cast<uint8>(value);
            return ptr + 2;
        } else {  // 处理大于 128 的数字
            ptr++;
            do {
                *ptr = static_cast<uint8>(value | 0x80); // 保留低7位, 并将第8位设置为1
                value >>= 7;
                ++ptr;
            } while (PROTOBUF_PREDICT_FALSE(value >= 0x80));
            *ptr++ = static_cast<uint8>(value);  // 最后它必然是小于128, 直接类型转换
            return ptr;
        }
    }
}

  PROTOBUF_ALWAYS_INLINE static uint8* UnsafeWriteSize(uint32 value,
                                                       uint8* ptr) {
    while (PROTOBUF_PREDICT_FALSE(value >= 0x80)) {
      *ptr = static_cast<uint8>(value | 0x80);
      value >>= 7;
      ++ptr;
    }
    *ptr++ = static_cast<uint8>(value);
    return ptr;
  }
```

#### Details
```c++

// Write a 32-bit little-endian integer.
void WriteLittleEndian32(uint32 value) {
  cur_ = impl_.EnsureSpace(cur_);
  SetCur(WriteLittleEndian32ToArray(value, Cur()));
}

// Write a 64-bit little-endian integer.
void WriteLittleEndian64(uint64 value) {
  cur_ = impl_.EnsureSpace(cur_);
  SetCur(WriteLittleEndian64ToArray(value, Cur()));
}


inline void CodedOutputStream::WriteVarint32(uint32 value) {
  cur_ = impl_.EnsureSpace(cur_);
  SetCur(WriteVarint32ToArray(value, Cur()));
}

inline uint8* CodedOutputStream::WriteVarint32ToArray(uint32 value,
                                                      uint8* target) {
  return EpsCopyOutputStream::UnsafeVarint(value, target); // 调用上面的 EpsCopyOutputStream::UnsafeVarint
}

// Like WriteLittleEndian32()  but writing directly to the target array.
inline uint8* CodedOutputStream::WriteLittleEndian32ToArray(uint32 value,
                                                            uint8* target) {
#if defined(PROTOBUF_LITTLE_ENDIAN) // 怎么设置的??
  memcpy(target, &value, sizeof(value));
#else
  target[0] = static_cast<uint8>(value);
  target[1] = static_cast<uint8>(value >> 8);
  target[2] = static_cast<uint8>(value >> 16);
  target[3] = static_cast<uint8>(value >> 24);
#endif
  return target + sizeof(value);
}
```

# ZeroCopy{Input,Output}Stream

- src/google/protobuf/io/zero_copy_stream_impl_lite.h
- src/google/protobuf/io/zero_copy_stream_impl.h

```
ZeroCopyInputStream
  |_ GzipInputStream
  |_ ArrayInputStream
  |_ CopyingInputStreamAdaptor
  |_ LimitingInputStream
  |_ FileInputStream
  |_ IstreamInputStream
  |_ ConcatenatingInputStream

ZeroCopyOutputStream
  |_ GzipOutputStream
  |_ ArrayOutputStream
  |_ StringOutputStream
  |_ CopyingOutputStreamAdaptor
  |_ OstreamOutputStream
```

- StringOutputStream

```c++
// A ZeroCopyOutputStream which appends bytes to a string.
class PROTOBUF_EXPORT StringOutputStream : public ZeroCopyOutputStream {
 public:
  // Create a StringOutputStream which appends bytes to the given string.
  // The string remains property of the caller, but it is mutated in arbitrary
  // ways and MUST NOT be accessed in any way until you're done with the
  // stream. Either be sure there's no further usage, or (safest) destroy the
  // stream before using the contents.
  //
  // Hint:  If you call target->reserve(n) before creating the stream,
  //   the first call to Next() will return at least n bytes of buffer
  //   space.
  explicit StringOutputStream(std::string* target);
  ~StringOutputStream() override = default;

  // implements ZeroCopyOutputStream ---------------------------------
  bool Next(void** data, int* size) override;
  void BackUp(int count) override;
  int64_t ByteCount() const override;

 private:
  static constexpr size_t kMinimumSize = 16;

  std::string* target_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(StringOutputStream);
};
```
