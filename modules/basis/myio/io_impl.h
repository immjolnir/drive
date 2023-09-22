#pragma once

// io.md
// How cout is implemented?
// https://stackoverflow.com/questions/72695497/how-is-stdcout-implemented

/*
- ./libstdc++-v3/include/std/iostream

namespace std
{
    extern istream cin;   // Linked to standard input
    extern ostream cout;  // Linked to standard output

    // For construction of filebuffers for cout, cin, cerr, clog et. al.
    static ios_base::Init __ioinit;  // 这个是 static? why?
}  // namespace std

// - ./libstdc++-v3/include/bits/ios_base.h
class ios_base {
    class Init {
        friend class ios_base;

      public:
        Init();
        ~Init();

      private:
        static _Atomic_word _S_refcount;
        static bool _S_synced_with_stdio;
    };
};

ios_base::Init::Init() {
    if (__gnu_cxx::__exchange_and_add_dispatch(&_S_refcount, 1) == 0) {
        // Standard streams default to synced with "C" operations.
        _S_synced_with_stdio = true;

        new (&buf_cout_sync) stdio_sync_filebuf<char>(stdout);
        new (&buf_cin_sync) stdio_sync_filebuf<char>(stdin);
        new (&buf_cerr_sync) stdio_sync_filebuf<char>(stderr);

        // The standard streams are constructed once only and never
        // destroyed.
        new (&cout) ostream(&buf_cout_sync);  // 通过这种方式实现的吧？？
        new (&cin) istream(&buf_cin_sync);
        new (&cerr) ostream(&buf_cerr_sync);
        new (&clog) ostream(&buf_cerr_sync);
        cin.tie(&cout);
        cerr.setf(ios_base::unitbuf);
        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // 455. cerr::tie() and wcerr::tie() are overspecified.
        cerr.tie(&cout);
        // NB: Have to set refcount above one, so that standard
        // streams are not re-initialized with uses of ios_base::Init
        // besides <iostream> static object, ie just using <ios> with
        // ios_base::Init objects.
        __gnu_cxx::__atomic_add_dispatch(&_S_refcount, 1);
    }
}
*/

// use std::stringstream as the buf
#include <sstream>

// forward declaring
namespace my
{
    typedef int _Atomic_word;

    class mystream;

    class ios_base {
      public:
        class Init {
            friend class ios_base;

          public:
            Init();
            ~Init();

            // Uncomment it for production
            // private:
            static _Atomic_word _S_refcount;
            static bool _S_synced_with_stdio;
        };
    };
}  // namespace my

namespace my
{
    class mystream {
      public:
        /**
         * @brief Base constructor
         * This ctor is almost never called by the user directly, rather from derived classes' inialization lists,
         * which pass a point to their own stream buffer.
         */
        mystream(std::stringstream* buf);

        // mystream();

        /**
         * @brief Base destructor
         *  This does very little apart from providing a virtual base dtor.
         */
        virtual ~mystream();

        template <typename T>
        void add(T&& t) {
            (*_buf) << t;
        }

        std::string to_string() const { return _buf->str(); }

      protected:
        /*
         *  @brief Empty.
         *
         *  The default constructor does nothing and is not normally
         *  accessible to users.
         *
         * See globals.cpp
         */
        mystream();  // protected ctor cannot construct an object.

        // mystream() = delete;  // init(0)
        // The copy constructor is protected, and is deleted. Output streams are not copyable.
        // https://en.cppreference.com/w/cpp/io/basic_ostream/basic_ostream
        mystream(const mystream&) = delete;
#if __cplusplus >= 201103L
        mystream(const mystream&&) = delete;
        mystream& operator=(const mystream&) = delete;
        mystream& operator=(mystream&& __rhs) = delete;
#endif

      public:
        std::stringstream* _buf = nullptr;
    };
}  // namespace my