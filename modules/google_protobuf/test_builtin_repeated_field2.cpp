#include <glog/logging.h>
#include <gtest/gtest.h>

#include <google/protobuf/arena.h>

#include <type_traits>

/*
Example from src/google/protobuf/repeated_field.h
*/
namespace google2 {
namespace protobuf {
template <typename Element>
class RepeatedField final {
  public:
    constexpr RepeatedField();  // How to use it when declaring the ctor with constexpr??

    template <typename Iter>
    RepeatedField(Iter begin, const Iter& end);
    ~RepeatedField();

    void Add(const Element& value);

    // Append elements in the range [begin, end) after reserving
    // the appropriate number of elements.
    template <typename Iter>
    void Add(Iter begin, Iter end);

    // Reserve space to expand the field to at least the given size.  This only
    // resizes the pointer array; it doesn't allocate any objects.  If the
    // array is grown, it will always be at least doubled in size.
    void Reserve(int new_size);

    int source = 0;

  private:
    /*
    : warning: 'template<class _Tp> struct std::is_pod' is deprecated: use is_standard_layout && is_trivial instead
    [-Wdeprecated-declarations]
    template <int = 0, bool = std::is_pod<Element>::value>
    */
    template <int = 0, bool = std::is_standard_layout<Element>::value && std::is_trivial<Element>::value>
    class FastAdderImpl {
      public:
        explicit FastAdderImpl(RepeatedField* rf) : repeated_field_(rf) {
            index_ = repeated_field_->current_size_;
            capacity_ = repeated_field_->total_size_;
            buffer_ = repeated_field_->unsafe_elements();
        }

        ~FastAdderImpl() { repeated_field_->current_size_ = index_; }

        void Add(Element val) {
            if (index_ == capacity_) {
                repeated_field_->current_size_ = index_;
                repeated_field_->Reserve(index_ + 1);
                capacity_ = repeated_field_->total_size_;
                buffer_ = repeated_field_->unsafe_elements();
            }
            buffer_[index_++] = val;
        }

      private:
        RepeatedField* repeated_field_;
        int index_;
        int capacity_;
        Element* buffer_;
    };

    // FastAdder is a wrapper for adding fields. The specialization above handles
    // POD types more efficiently than RepeatedField.
    template <int I>
    class FastAdderImpl<I, false> {
      public:
        explicit FastAdderImpl(RepeatedField* rf) : repeated_field_(rf) {}

        void Add(const Element& val) { repeated_field_->Add(val); }

      private:
        RepeatedField* repeated_field_;
    };

    using FastAdder = FastAdderImpl<>;  // What's this? using template class's default parameters?
    std::vector<Element> _buffer;
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
        google::protobuf::Arena* arena;
        // Here we declare a huge array as a way of approximating C's "flexible
        // array member" feature without relying on undefined behavior.
        Element elements[(std::numeric_limits<int>::max() - 2 * sizeof(google::protobuf::Arena*)) / sizeof(Element)];
    };

    static constexpr size_t kRepHeaderSize = offsetof(Rep, elements);

    // If total_size_ == 0 this points to an Arena otherwise it points to the
    // elements member of a Rep struct. Using this invariant allows the storage of
    // the arena pointer without an extra allocation in the constructor.
    void* arena_or_elements_;

    // Return pointer to elements array.
    // pre-condition: the array must have been allocated.
    Element* elements() const {
        CHECK_GT(total_size_, 0);
        // Because of above pre-condition this cast is safe.
        return unsafe_elements();
    }

    // Return pointer to elements array if it exists otherwise either null or
    // a invalid pointer is returned. This only happens for empty repeated fields,
    // where you can't dereference this pointer anyway (it's empty).
    Element* unsafe_elements() const { return static_cast<Element*>(arena_or_elements_); }

    // Return pointer to the Rep struct.
    // pre-condition: the Rep must have been allocated, ie elements() is safe.
    Rep* rep() const {
        char* addr = reinterpret_cast<char*>(elements()) - offsetof(Rep, elements);
        return reinterpret_cast<Rep*>(addr);
    }

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
                    e->~Element();
                }
            }
            if (rep->arena == NULL) {
#if defined(__GXX_DELETE_WITH_SIZE__) || defined(__cpp_sized_deallocation)
                const size_t bytes = size * sizeof(*e) + kRepHeaderSize;
                ::operator delete(static_cast<void*>(rep), bytes);
#else
                ::operator delete(static_cast<void*>(rep));
#endif
            }
        }
    }
};

namespace internal {

// This is a helper template to copy an array of elements efficiently when they
// have a trivial copy constructor, and correctly otherwise. This really
// shouldn't be necessary, but our compiler doesn't optimize std::copy very
// effectively.
template <typename Element,
          bool HasTrivialCopy = std::is_standard_layout<Element>::value && std::is_trivial<Element>::value>
struct ElementCopier {
    void operator()(Element* to, const Element* from, int array_size);
};

}  // namespace internal

template <typename Element>
constexpr RepeatedField<Element>::RepeatedField() : current_size_(0), total_size_(0), arena_or_elements_(nullptr) {}

template <typename Element>
template <typename Iter>
RepeatedField<Element>::RepeatedField(Iter begin, const Iter& end)
  : current_size_(0), total_size_(0), arena_or_elements_(nullptr) {
    Add(begin, end);
}

template <typename Element>
template <typename Iter>
inline void RepeatedField<Element>::Add(Iter begin, Iter end) {
    FastAdder fast_adder(this);
    for (; begin != end; ++begin) fast_adder.Add(*begin);
}

template <typename Element>
RepeatedField<Element>::~RepeatedField() {
    if (total_size_ > 0) {
        InternalDeallocate(rep(), total_size_);
    }
}

template <typename Element>
inline void RepeatedField<Element>::MoveArray(Element* to, Element* from, int array_size) {
    CopyArray(to, from, array_size);
}

template <typename Element>
inline void RepeatedField<Element>::CopyArray(Element* to, const Element* from, int array_size) {
    internal::ElementCopier<Element>()(to, from, array_size);
}

namespace internal {

template <typename Element, bool HasTrivialCopy>
void ElementCopier<Element, HasTrivialCopy>::operator()(Element* to, const Element* from, int array_size) {
    std::copy(from, from + array_size, to);
}

template <typename Element>
struct ElementCopier<Element, true> {
    void operator()(Element* to, const Element* from, int array_size) {
        memcpy(to, from, static_cast<size_t>(array_size) * sizeof(Element));
    }
};

// kRepeatedFieldLowerClampLimit is the smallest size that will be allocated
// when growing a repeated field.
constexpr int kRepeatedFieldLowerClampLimit = 4;

// kRepeatedFieldUpperClampLimit is the lowest signed integer value that
// overflows when multiplied by 2 (which is undefined behavior). Sizes above
// this will clamp to the maximum int value instead of following exponential
// growth when growing a repeated field.
constexpr int kRepeatedFieldUpperClampLimit = (std::numeric_limits<int>::max() / 2) + 1;

// A utility function for logging that doesn't need any template types.
void LogIndexOutOfBounds(int index, int size);

template <typename Iter>
inline int CalculateReserve(Iter begin, Iter end, std::forward_iterator_tag) {
    return static_cast<int>(std::distance(begin, end));
}

template <typename Iter>
inline int CalculateReserve(Iter /*begin*/, Iter /*end*/, std::input_iterator_tag /*unused*/) {
    return -1;
}

template <typename Iter>
inline int CalculateReserve(Iter begin, Iter end) {
    typedef typename std::iterator_traits<Iter>::iterator_category Category;
    return CalculateReserve(begin, end, Category());
}

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
        CHECK_GT(new_size, kRepeatedFieldUpperClampLimit) << ": new_size is bigger than the allowed value";
        return std::numeric_limits<int>::max();
    }
}
}  // namespace internal

// Avoid inlining of Reserve(): new, copy, and delete[] lead to a significant
// amount of code bloat.
template <typename Element>
void RepeatedField<Element>::Reserve(int new_size) {
    if (total_size_ >= new_size) return;

    Rep* old_rep = total_size_ > 0 ? rep() : NULL;
    Rep* new_rep;
    new_size = internal::CalculateReserveSize(total_size_, new_size);
    CHECK_LE(static_cast<size_t>(new_size), (std::numeric_limits<size_t>::max() - kRepHeaderSize) / sizeof(Element))
      << ": Requested size is too large to fit into size_t.";

    size_t bytes = kRepHeaderSize + sizeof(Element) * static_cast<size_t>(new_size);

    new_rep = static_cast<Rep*>(::operator new(bytes));

    new_rep->arena = NULL;
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
        new (e) Element;
    }
    if (current_size_ > 0) {
        MoveArray(&elements()[0], old_rep->elements, current_size_);
    }

    // Likewise, we need to invoke destructors on the old array.
    InternalDeallocate(old_rep, old_total_size);
}
}  // namespace protobuf
}  // namespace google2

TEST(repeated_field, example) {}