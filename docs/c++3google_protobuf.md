# Protobuf

- Projects
  - [protobuf](https://github.com/protocolbuffers/protobuf)
  - [protoscope](https://github.com/protocolbuffers/protoscope)

- Learning code
  - modules/google_protobuf/

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
    Reserve(total_size_ + 1);
    elements()[size] = std::move(tmp);
  } else {
    elements()[size] = value; // 还是以数组的方式来添加的元素的
  }
  current_size_ = size + 1;
}

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


# SQLite 
- https://www.sqlite.org/fileformat.html
- https://www.sqlite.org/fileformat2.html#varint

# Arena
