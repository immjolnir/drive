# 

## `cin`, `cout` and `static __ioinit`

- gccmaster: ./libstdc++-v3/include/std/iostream

```
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @name Standard Stream Objects
   *
   *  The &lt;iostream&gt; header declares the eight <em>standard stream
   *  objects</em>.  For other declarations, see
   *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/io.html
   *  and the @link iosfwd I/O forward declarations @endlink
   *
   *  They are required by default to cooperate with the global C
   *  library's @c FILE streams, and to be available during program
   *  startup and termination. For more information, see the section of the
   *  manual linked to above.
  */
  ///@{
  extern istream cin;       /// Linked to standard input
  extern ostream cout;      /// Linked to standard output
  extern ostream cerr;      /// Linked to standard error (unbuffered)
  extern ostream clog;      /// Linked to standard error (buffered)

#ifdef _GLIBCXX_USE_WCHAR_T
  extern wistream wcin;     /// Linked to standard input
  extern wostream wcout;    /// Linked to standard output
  extern wostream wcerr;    /// Linked to standard error (unbuffered)
  extern wostream wclog;    /// Linked to standard error (buffered)
#endif
  ///@}

  // For construction of filebuffers for cout, cin, cerr, clog et. al.
  // 用于构建 cout, cin
  static ios_base::Init __ioinit; // 这个是 static? why?

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
```

这种全局对象: cin, cout  也是通过 extern 的方法来声明的.

From the link: https://gcc.gnu.org/onlinedocs/libstdc++/manual/io.html

To minimize the time you have to wait on the compiler, it's good to only include the headers you really need.
Many people simply include <iostream> when they don't need to -- and that can penalize your runtime as well.

Here are some tips on which header to use for which situations, starting with the simplest.

- `<iosfwd>` should be included whenever you simply need the name of an I/O-related class, such as "ofstream" or "basic_streambuf". Like the name implies, these are forward declarations. (A word to all you fellow old school programmers: trying to forward declare classes like "class istream;" won't work. Look in the <iosfwd> header if you'd like to know why.) For example,
```
    #include <iosfwd>

    class MyClass
    {
      ....
      std::ifstream&   input_file;
    };

    extern std::ostream& operator<< (std::ostream&, MyClass&);
```

- <istream> and <ostream> are the headers to include when you are using the overloaded >> and << operators, or any of the other abstract stream formatting functions. For example,
```
    #include <istream>

    std::ostream& operator<< (std::ostream& os, MyClass& c)
    {
       return os << c.data1() << c.data2();
    }
```   
The std::istream and std::ostream classes are the abstract parents of the various concrete implementations. If you are only using the interfaces, then you only need to use the appropriate interface header.

- <iostream> provides the eight standard global objects (cin, cout, etc).
```
    #include <ostream>
    #include <istream>

    namespace std
    {
        extern istream cin;
        extern ostream cout;
        ....

        // this is explained below
        static ios_base::Init __foo;    // not its real name
    }
```
Now, the runtime penalty mentioned previously: the global objects must be initialized before any of your own code uses them; this is guaranteed by the standard. Like any other global object, they must be initialized once and only once. This is typically done with a construct like the one above, and the nested class ios_base::Init is specified in the standard for just this reason.

How does it work? Because the header is included before any of your code, the __foo object is constructed before any of your objects. (Global objects are built in the order in which they are declared, and destroyed in reverse order.) The first time the constructor runs, the eight stream objects are set up.

The static keyword means that each object file compiled from a source file containing <iostream> will have its own private copy of __foo. There is no specified order of construction across object files (it's one of those pesky NP complete problems that make life so interesting), so one copy in each object file means that the stream objects are guaranteed to be set up before any of your code which uses them could run, thereby meeting the requirements of the standard.

The penalty, of course, is that after the first copy of __foo is constructed, all the others are just wasted processor time. The time spent is merely for an increment-and-test inside a function call, but over several dozen or hundreds of object files, that time can add up. (It's not in a tight loop, either.)

The lesson? Only include <iostream> when you need to use one of the standard objects in that source file; you'll pay less startup time. Only include the header files you need to in general; your compile times will go down when there's less parsing work to do.

## iosfwd
- ./libstdc++-v3/include/std/iosfwd

```
  *  Nearly all of the I/O classes are parameterized on the type of
   *  characters they read and write.  (The major exception is ios_base at
   *  the top of the hierarchy.)  This is a change from pre-Standard
   *  streams, which were not templates.


   *  Because properly forward-declaring these classes can be difficult, you
   *  should not do it yourself.  Instead, include the &lt;iosfwd&gt;
   *  header, which contains only declarations of all the I/O classes as
   *  well as the typedefs.  Trying to forward-declare the typedefs
   *  themselves (e.g., <code>class ostream;</code>) is not valid ISO C++.
   *

  /// Base class for @c char input streams.
  typedef basic_istream<char>       istream;

  /// Base class for @c char output streams.
  typedef basic_ostream<char>       ostream;

  /// Base class for @c char mixed input and output streams.
  typedef basic_iostream<char>      iostream;

  /// Class for @c char file buffers.
  typedef basic_filebuf<char>       filebuf;
```

## stringfwd
- ./libstdc++-v3/include/bits/stringfwd.h

```
  /// A string of @c char
  typedef basic_string<char>    string;

  /// A string of @c char16_t
  typedef basic_string<char16_t> u16string;
```

## ios_base::Init
- ./libstdc++-v3/include/bits/ios_base.h

```
    // 27.4.2.1.6  Class ios_base::Init
    // Used to initialize standard streams. In theory, g++ could use
    // -finit-priority to order this stuff correctly without going
    // through these machinations.
    class Init
    {
      friend class ios_base;
    public:
      Init();
      ~Init();

#if __cplusplus >= 201103L
      Init(const Init&) = default;
      Init& operator=(const Init&) = default;
#endif

    private:
      static _Atomic_word   _S_refcount;
      static bool       _S_synced_with_stdio;
    };
```

- libstdc++-v3/src/c++11/ios.cc

```
  _Atomic_word ios_base::Init::_S_refcount;

  bool ios_base::Init::_S_synced_with_stdio = true;
```

- libstdc++-v3/src/c++98/ios_init.cc

```
  extern istream cin;
  extern ostream cout;
  extern ostream cerr;
  extern ostream clog;

  ios_base::Init::Init()
  {
    if (__gnu_cxx::__exchange_and_add_dispatch(&_S_refcount, 1) == 0)
      {
    // Standard streams default to synced with "C" operations.
    _S_synced_with_stdio = true;

    new (&buf_cout_sync) stdio_sync_filebuf<char>(stdout);
    new (&buf_cin_sync) stdio_sync_filebuf<char>(stdin);
    new (&buf_cerr_sync) stdio_sync_filebuf<char>(stderr);

    // The standard streams are constructed once only and never
    // destroyed.
    new (&cout) ostream(&buf_cout_sync); // 通过这种方式实现的吧？？
    new (&cin) istream(&buf_cin_sync);
    new (&cerr) ostream(&buf_cerr_sync);
    new (&clog) ostream(&buf_cerr_sync);
    cin.tie(&cout);
    cerr.setf(ios_base::unitbuf);
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 455. cerr::tie() and wcerr::tie() are overspecified.
    cerr.tie(&cout);

#ifdef _GLIBCXX_USE_WCHAR_T
    new (&buf_wcout_sync) stdio_sync_filebuf<wchar_t>(stdout);
    new (&buf_wcin_sync) stdio_sync_filebuf<wchar_t>(stdin);
    new (&buf_wcerr_sync) stdio_sync_filebuf<wchar_t>(stderr);

    new (&wcout) wostream(&buf_wcout_sync);
    new (&wcin) wistream(&buf_wcin_sync);
    new (&wcerr) wostream(&buf_wcerr_sync);
    new (&wclog) wostream(&buf_wcerr_sync);
    wcin.tie(&wcout);
    wcerr.setf(ios_base::unitbuf);
    wcerr.tie(&wcout);
#endif

    // NB: Have to set refcount above one, so that standard
    // streams are not re-initialized with uses of ios_base::Init
    // besides <iostream> static object, ie just using <ios> with
    // ios_base::Init objects.
    __gnu_cxx::__atomic_add_dispatch(&_S_refcount, 1);
      }
  }
```


- libstdc++-v3/src/c++98/globals_io.cc

// On AIX, and perhaps other systems, library initialization order is
// not guaranteed.  For example, the static initializers for the main
// program might run before the static initializers for this library.
// That means that we cannot rely on static initialization in the
// library; there is no guarantee that things will get initialized in
// time.  This file contains definitions of all global variables that
// require initialization as arrays of characters.

// NB: asm directives can rename these non-exported, namespace
// __gnu_cxx symbols into exported, namespace std symbols with the
// appropriate symbol version name.
// The rename syntax is
//   asm (".symver currentname,oldname@@GLIBCXX_3.2")
// In macro form:
// _GLIBCXX_ASM_SYMVER(currentname, oldname, GLIBCXX_3.2)


```
  // Standard stream objects.
  // NB: Iff <iostream> is included, these definitions become wonky.
  typedef char fake_istream[sizeof(istream)]
  __attribute__ ((aligned(__alignof__(istream))));
  typedef char fake_ostream[sizeof(ostream)]
  __attribute__ ((aligned(__alignof__(ostream))));
  fake_istream cin;
  fake_ostream cout;
  fake_ostream cerr;
  fake_ostream clog;
```

## Variable Initialization Order
- [Change library load order at run time (like LD_PRELOAD but during execution)](https://stackoverflow.com/questions/3067545/change-library-load-order-at-run-time-like-ld-preload-but-during-execution)

AFAIK, that is not possible. The general rule is that if the same symbol appears in two libraries, `ld.so` will favor the library that was loaded first. `LD_PRELOAD` works by making sure the specified libraries are loaded before any implicitly loaded libraries.

So once execution has started, all implicitly loaded libraries will have been loaded and therefore it's too late to load your library before them.



## LTO

Link Time Optimization (LTO) gives GCC the capability of dumping its internal representation (GIMPLE) to disk, so that all the different compilation units that make up a single executable can be optimized as a single module.

- [Is there a reason why not to use link-time optimization (LTO)?](https://stackoverflow.com/questions/23736507/is-there-a-reason-why-not-to-use-link-time-optimization-lto)

- https://llvm.org/docs/LinkTimeOptimization.html

## linking dynamic library will change the initialization order of the static and global variables?

## [Static Initialization Order Fiasco](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)

Within a single compilation unit, `static variables` are initialized in the same order as they are defined in the source (this is called `Ordered Dynamic Initialization`). Across compilation units, however, the order is undefined: you don’t know if a static variable defined in `a.cpp` will be initialized before or after one in `b.cpp`.

## what kinds of variables will be created at compile time in c++?

* https://www.learncpp.com/cpp-tutorial/compile-time-constants-constant-expressions-and-constexpr/
  - A `constexpr` (which is short for “constant expression”) variable can only be a compile-time constant. If the initialization value of a `constexpr` variable is not a constant expression, the compiler will error.




## nm comm
nm -D --defined-only lib.a

nm -g


## Why the `std::cout`, `std::cin` are defined in `globals_io.cc`?

- https://stackoverflow.com/questions/67365282/where-is-stdcout-defined

How libstdc++ used by gcc does it:

Storage for cout is defined as a global variable of type `fake_ostream` which is presumably constructible without problems. https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/src/c%2B%2B98/globals_io.cc

Then during library initialization rewritten with a __placement new__ using the explicit constructor. https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/src/c%2B%2B98/ios_init.cc

Other compilers have their own libraries and may use different tricks. Examining the source of libc++ used by clang left as exercise for the reader.

> Q: Thank you for answer. It is perfect, but I still don't get it. I see that there is created new type ( this is char[sizeof(ostream)] ) and next there is defined new object ( name cout ) using that new type. In other file there is declared cout using extern, but with type... ostream. How? When I tried do this I get error "redeclaration of xxxx ( my object name ) with different type: std:ostream vs char[272]" 

> A: I think that's what the comment Iff <iostream> is included, these definitions become wonky in globals_io.cc is about. The real declaration (extern ostream cout) and the fake definition (fake_ostream cout) should not meet in one compilation unit. And if you have defined something called cout with the right size in one file and another file is asking for an extern cout, the linker will connect those two. For data, only names are checked during linking, there is no way to compare types.

search for "compilation and linking C++", there are better explanations than mine.

- https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/src/c%2B%2B98/ios_base_init.h
```
// This is only in a header so we can use the system_header pragma,
// to suppress the warning caused by using a reserved init_priority.
#pragma GCC system_header

// If the target supports init priorities, set up a static object in the
// compiled library to perform the <iostream> initialization once and
// sufficiently early (so that it happens before any other global
// constructor when statically linking with libstdc++.a), instead of
// doing so in (each TU that includes) <iostream>.
// This needs to be done in the same TU that defines the stream objects.
#if _GLIBCXX_USE_INIT_PRIORITY_ATTRIBUTE
static ios_base::Init __ioinit __attribute__((init_priority(90)));
#endif
```

See modules/basis/myio
