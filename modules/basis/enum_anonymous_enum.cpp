#include <gtest/gtest.h>

/**
 * https://stackoverflow.com/questions/7147008/the-usage-of-anonymous-enums

What is the purpose of anonymous enum declarations such as:

    enum { color = 1 };

Why not just declare int color = 1?

Answer:

1. That's a so-called enum trick for declaring a compile-time integer constant. It's advantage is it guarantees that no
variable is instantiated and therefore there's no runtime overhead. Most compilers introduce no overhead with integer
constants anyway.

2. Enums don't take up any space and are immutable.

If you used
    const int color = 1;
then you would solve the mutability issue but if someone took the address of color
    (const int* p = &color;)
then space for it would have to be allocated.
This may not be a big deal but unless you explicitly want people to be able to take the address of color you might as
well prevent it.

Also when declaring a constant field in a class then it will have to be static const (not true for modern C++) and not
all compilers support inline initialization of static const members.

* https://www.linuxtopia.org/online_books/programming_books/thinking_in_c++/Chapter08_023.html
* enum hack
*/

class Foo {
    enum { color = 1 };
};

class Bar {
    const int color = 1;
};

class Baz {
    static int color;
};

int Baz::color = 1;

TEST(enum, anonymous_enum_size) {
    EXPECT_EQ(1, sizeof(Foo));  // anonymous enum doesn't take up any space.
    EXPECT_EQ(4, sizeof(Bar));  // const int does take 4-bytes space.
    EXPECT_EQ(1, sizeof(Baz));  // static member doesn't take up any space too, which allocated at compile time.
}

// In Practice
// loki-lib: include/loki/SmartPtr.h

// loki-lib: include/loki/TypeManip.h
////////////////////////////////////////////////////////////////////////////////
// class template Select
// Selects one of two types based upon a boolean constant
// Invocation: Select<flag, T, U>::Result
// where:
// flag is a compile-time boolean constant
// T and U are types
// Result evaluates to T if flag is true, and to U otherwise.
////////////////////////////////////////////////////////////////////////////////
template <bool flag, typename T, typename U>
struct Select {
    typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U> {
    typedef U Result;
};

template <class P>
class RefCounted {
  public:
    int Count() const { return *pCount_; }

  protected:
    RefCounted() : pCount_(static_cast<uintptr_t*>(new uintptr_t)) {
        assert(pCount_ != 0);
        *pCount_ = 1;
    }

    RefCounted(const RefCounted& rhs) : pCount_(rhs.pCount_) {}

    // MWCW lacks template friends, hence the following kludge
    template <typename P1>
    RefCounted(const RefCounted<P1>& rhs) : pCount_(reinterpret_cast<const RefCounted&>(rhs).pCount_) {}

    P Clone(const P& val) {
        ++*pCount_;
        return val;
    }

    bool Release(const P&) {
        if (!--*pCount_) {
            delete pCount_;
            pCount_ = NULL;
            return true;
        }
        return false;
    }

    void Swap(RefCounted& rhs) { std::swap(pCount_, rhs.pCount_); }

    enum { destructiveCopy = false };

  private:
    // Data
    uintptr_t* pCount_;  // 一定要用 new 吗？
};

template <class P>
class DestructiveCopy {
  public:
    int Count() const { return 1; }

  protected:
    DestructiveCopy() {}

    template <class P1>
    DestructiveCopy(const DestructiveCopy<P1>&) {}

    template <class P1>
    static P Clone(const P1& val) {
        P result(val);
        const_cast<P1&>(val) = P1();
        return result;
    }

    template <class P1>
    static P Clone(P1& val) {
        P result(val);
        val = P1();
        return result;
    }

    static bool Release(const P&) { return true; }

    static void Swap(DestructiveCopy&) {}

    enum { destructiveCopy = true };
};

template <class T>
class DefaultSPStorage {
  public:
    typedef T* StoredType;       // the type of the pointee_ object
    typedef T* InitPointerType;  /// type used to declare OwnershipPolicy type.
    typedef T* PointerType;      // type returned by operator->
    typedef T& ReferenceType;    // type returned by operator*

  protected:
    DefaultSPStorage() : pointee_(Default()) {}

    // The storage policy doesn't initialize the stored pointer
    //     which will be initialized by the OwnershipPolicy's Clone fn
    DefaultSPStorage(const DefaultSPStorage&) : pointee_(0) {}

    template <class U>
    DefaultSPStorage(const DefaultSPStorage<U>&) : pointee_(0) {}

    explicit DefaultSPStorage(const StoredType& p) : pointee_(p) {}

    PointerType operator->() const { return pointee_; }

    ReferenceType operator*() const { return *pointee_; }

    void Swap(DefaultSPStorage& rhs) { std::swap(pointee_, rhs.pointee_); }

    // Accessors
    template <class F>
    friend typename DefaultSPStorage<F>::PointerType GetImpl(const DefaultSPStorage<F>& sp);

    template <class F>
    friend const typename DefaultSPStorage<F>::StoredType& GetImplRef(const DefaultSPStorage<F>& sp);

    template <class F>
    friend typename DefaultSPStorage<F>::StoredType& GetImplRef(DefaultSPStorage<F>& sp);

    // Destroys the data stored
    // (Destruction might be taken over by the OwnershipPolicy)
    //
    // If your compiler gives you a warning in this area while
    // compiling the tests, it is on purpose, please ignore it.
    void Destroy() { delete pointee_; }

    // Default value to initialize the pointer
    static StoredType Default() { return 0; }

  private:
    // Data
    StoredType pointee_;
};

template <class T>
inline typename DefaultSPStorage<T>::PointerType GetImpl(const DefaultSPStorage<T>& sp) {
    return sp.pointee_;
}

template <class T>
inline const typename DefaultSPStorage<T>::StoredType& GetImplRef(const DefaultSPStorage<T>& sp) {
    return sp.pointee_;
}

template <class T>
inline typename DefaultSPStorage<T>::StoredType& GetImplRef(DefaultSPStorage<T>& sp) {
    return sp.pointee_;
}

template <class T>
class HeapStorage {
  public:
    typedef T* StoredType;       /// the type of the pointee_ object
    typedef T* InitPointerType;  /// type used to declare OwnershipPolicy type.
    typedef T* PointerType;      /// type returned by operator->
    typedef T& ReferenceType;    /// type returned by operator*

  protected:
    HeapStorage() : pointee_(Default()) {}

    // The storage policy doesn't initialize the stored pointer
    //     which will be initialized by the OwnershipPolicy's Clone fn
    HeapStorage(const HeapStorage&) : pointee_(0) {}

    template <class U>
    HeapStorage(const HeapStorage<U>&) : pointee_(0) {}

    explicit HeapStorage(const StoredType& p) : pointee_(p) {}

    PointerType operator->() const { return pointee_; }

    ReferenceType operator*() const { return *pointee_; }

    void Swap(HeapStorage& rhs) { std::swap(pointee_, rhs.pointee_); }

    // Accessors
    template <class F>
    friend typename HeapStorage<F>::PointerType GetImpl(const HeapStorage<F>& sp);

    template <class F>
    friend const typename HeapStorage<F>::StoredType& GetImplRef(const HeapStorage<F>& sp);

    template <class F>
    friend typename HeapStorage<F>::StoredType& GetImplRef(HeapStorage<F>& sp);

    // Destroys the data stored
    // (Destruction might be taken over by the OwnershipPolicy)
    void Destroy() {
        if (0 != pointee_) {
            pointee_->~T();
            ::free(pointee_);
        }
    }

    // Default value to initialize the pointer
    static StoredType Default() { return 0; }

  private:
    // Data
    StoredType pointee_;
};

template <class T>
inline typename HeapStorage<T>::PointerType GetImpl(const HeapStorage<T>& sp) {
    return sp.pointee_;
}

template <class T>
inline const typename HeapStorage<T>::StoredType& GetImplRef(const HeapStorage<T>& sp) {
    return sp.pointee_;
}

template <class T>
inline typename HeapStorage<T>::StoredType& GetImplRef(HeapStorage<T>& sp) {
    return sp.pointee_;
}

template <typename T, template <class> class OwnershipPolicy = RefCounted,
          template <class> class StoragePolicy = DefaultSPStorage>
class SmartPtr : public StoragePolicy<T>, public OwnershipPolicy<typename StoragePolicy<T>::InitPointerType> {
    // typedef  Self;
    typedef StoragePolicy<T> SP;

    typedef OwnershipPolicy<typename StoragePolicy<T>::InitPointerType> OP;
    typedef typename Select<OP::destructiveCopy, SmartPtr, const SmartPtr>::Result CopyArg;

    typedef typename SP::PointerType PointerType;
    typedef typename SP::StoredType StoredType;
    typedef typename SP::ReferenceType ReferenceType;

  private:
    struct NeverMatched {};

    typedef const StoredType& ImplicitArg;
    typedef typename Select<false, const StoredType&, NeverMatched>::Result ExplicitArg;

  public:
    explicit SmartPtr(ExplicitArg p) : SP(p) {}

    SmartPtr(ImplicitArg p) : SP(p) {}

    SmartPtr(CopyArg& rhs) : SP(rhs), OP(rhs) {
        std::cout << "copy ctor" << std::endl;
        GetImplRef(*this) = OP::Clone(GetImplRef(rhs));
    }

    SmartPtr& operator=(CopyArg& rhs) {
        std::cout << "operator=" << std::endl;
        SmartPtr temp(rhs);
        temp.Swap(*this);
        return *this;
    }

    void Swap(SmartPtr& rhs) {
        OP::Swap(rhs);
        SP::Swap(rhs);
    }

    ~SmartPtr() {
        if (OP::Release(GetImpl(*static_cast<SP*>(this)))) {
            SP::Destroy();
        }
    }

    PointerType operator->() { return SP::operator->(); }

    const PointerType operator->() const { return SP::operator->(); }

    ReferenceType operator*() { return SP::operator*(); }

    const ReferenceType operator*() const { return SP::operator*(); }
};

class TestClass {
  public:
    TestClass() { ++instances; }

    ~TestClass() { --instances; }

  public:
    static int instances;
};

int TestClass::instances = 0;

TEST(enum, anonymous_enum_with_ref_counted) {
    typedef SmartPtr<TestClass, RefCounted, DefaultSPStorage> P;
    P p0(new TestClass);
    EXPECT_EQ(1, TestClass::instances);

    P p1(p0);
    EXPECT_EQ(1, TestClass::instances);
    EXPECT_EQ(2, p0.Count());
    EXPECT_EQ(2, p1.Count());
}

TEST(enum, anonymous_enum_with_destructiveCopy) {
    typedef SmartPtr<TestClass, DestructiveCopy, DefaultSPStorage> P;
    P p0(new TestClass);
    EXPECT_EQ(1, TestClass::instances);

    P p1(p0);
    EXPECT_EQ(1, TestClass::instances);  // Why it still be 1?
    EXPECT_EQ(1, p0.Count());
    EXPECT_EQ(1, p1.Count());
}

TEST(enum, anonymous_enum_with_ref_counted_assignment_operator) {
    typedef SmartPtr<TestClass, RefCounted, DefaultSPStorage> P;
    P p0(new TestClass);
    EXPECT_EQ(1, TestClass::instances);

    P p1 = p0;  // using operator =, but why not call it?
    EXPECT_EQ(1, TestClass::instances);
    EXPECT_EQ(2, p0.Count());
    EXPECT_EQ(2, p1.Count());
}

TEST(enum, anonymous_enum_with_destructiveCopy_assignment_operator) {
    typedef SmartPtr<TestClass, DestructiveCopy, DefaultSPStorage> P;
    P p0(new TestClass);
    EXPECT_EQ(1, TestClass::instances);

    P p1 = p0;                           // using operator =, why not call it?
    EXPECT_EQ(2, TestClass::instances);  // Why it still be 1?
    EXPECT_EQ(1, p0.Count());
    EXPECT_EQ(1, p1.Count());
}