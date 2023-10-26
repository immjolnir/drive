#include <gtest/gtest.h>

// https://serde.rs/
// Serde is a framework for serializing and deserializing Rust data structures efficiently and generically.
TEST(Serde, vector_char) {
    std::vector<unsigned char> uvec{'a', 'b', 'c'};
    // Serialize the vector into the underlying
    // https://en.cppreference.com/w/cpp/string/basic_string/basic_string
    // Call
    // basic_string( const CharT* s, size_type count, const Allocator& alloc = Allocator());
    std::string str(reinterpret_cast<char*>(uvec.data()), uvec.size());  // No data copy
    EXPECT_EQ(uvec.size(), str.size());
    EXPECT_EQ("abc", str);

    // Deserialize the string into mat
    // Construct the cv::Mat by calling one of them:
    // Mat (int rows, int cols, int type, void *data, size_t step=AUTO_STEP)
    // Mat (Size size, int type, void *data, size_t step=AUTO_STEP)
    // https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html
    //
    // reinterpret_cast can convert between unrelated pointer types, but can't remove const or volatile qualifiers. You
    // need const_cast for that.
    // https://stackoverflow.com/questions/28149663/reinterpret-cast-from-type-const-char-to-type-flashstringhelper-casts-aw
    // cv::Mat decoded_roi(roi_size, CV_8UC1, reinterpret_cast<unsigned char*>(const_cast<char*>(str.data())));

    // https://en.cppreference.com/w/cpp/container/vector/vector
    // vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    std::vector<unsigned char> devec(str.begin(), str.end());
    EXPECT_EQ(uvec.size(), devec.size());
    EXPECT_EQ('a', devec[0]);
    EXPECT_EQ('b', devec[1]);
    EXPECT_EQ('c', devec[2]);
}

TEST(Serde, vector_short) {
    EXPECT_EQ(2, sizeof(short));
    EXPECT_EQ(4, sizeof(int));
    EXPECT_EQ(24, sizeof(std::vector<short>));

    //          short: min=0x8000(-32768), max=0x7fff(32767)
    // unsigned short: min=0, max=0xffff(65535)
    // 补码，反码，原码
    //  error: narrowing conversion of '32768' from 'int' to 'short int' [-Wnarrowing]
    // std::vector<short> uvec{0x8000, 0x7fff, 0xffff};
    std::vector<short> uvec;

    uvec.emplace_back(0x8000);
    uvec.emplace_back(0x7fff);
    uvec.emplace_back(0xffff);
    EXPECT_EQ(3, uvec.size());

    EXPECT_EQ(-32768, uvec[0]);
    EXPECT_EQ(32767, uvec[1]);
    EXPECT_EQ(-1, uvec[2]);
    {
        // Serialize
        std::string str(reinterpret_cast<char*>(uvec.data()), uvec.size());  // No data copy
        EXPECT_EQ(uvec.size(), str.size());
        // EXPECT_EQ("abc", str);

        // Deserialize
        std::vector<short> devec(str.begin(), str.end());
        EXPECT_EQ(uvec.size(), devec.size());
        EXPECT_EQ(-32768, devec[0]);  // ERR, 0
        EXPECT_EQ(32767, devec[1]);   // ERR, -128
        EXPECT_EQ(-1, devec[2]);      // OK, but why?
        // So the above is an incorrect serialization method on short type.
    }

    // Fix
    {
        // using memcpy to serialize the vector<short>
        int data_size = uvec.size() * sizeof(decltype(uvec)::value_type);
        std::string str;
        str.reserve(data_size);  // reserve vs resize
        // str2.resize(data_size);
        // Copy uvec to str
        std::memcpy(str.data(), uvec.data(), data_size);

        // Deserialize
        std::vector<short> devec;
        // devec.reserve(uvec.size()); // won't change the vector's size.
        devec.resize(uvec.size());  // Change the vector's size
        // Copy str to devec
        std::memcpy(devec.data(), str.data(), data_size);
        EXPECT_EQ(3, devec.size());  // actual=0 when using vector's reserve method
        EXPECT_EQ(-32768, devec[0]);
        EXPECT_EQ(32767, devec[1]);
        EXPECT_EQ(-1, devec[2]);
        // Maybe, we can choose
        // std::u16string
        // std::u32string
        // https://en.cppreference.com/w/cpp/string
        //
    }
}

TEST(Serde, vector_reserve_vs_resize) {
    EXPECT_EQ(2, sizeof(short));
    EXPECT_EQ(4, sizeof(int));
    EXPECT_EQ(24, sizeof(std::vector<short>));

    int data_size = 1024;
    {  // reserve
        std::string storage;
        storage.reserve(data_size);
        EXPECT_EQ(0, storage.size());
    }
    {  // resize
        std::string storage;
        storage.resize(data_size);
        EXPECT_EQ(data_size, storage.size());
    }
}

TEST(Serde, vector_short_by_istream) {
    //          short: min=0x8000(-32768), max=0x7fff(32767)
    // unsigned short: min=0, max=0xffff(65535)
    // 补码，反码，原码
    //  error: narrowing conversion of '32768' from 'int' to 'short int' [-Wnarrowing]
    // std::vector<short> uvec{0x8000, 0x7fff, 0xffff};
    std::vector<short> uvec;
    uvec.emplace_back(0x8000);
    uvec.emplace_back(0x7fff);
    uvec.emplace_back(0xffff);

    EXPECT_EQ(3, uvec.size());
    EXPECT_EQ(-32768, uvec[0]);
    EXPECT_EQ(32767, uvec[1]);
    EXPECT_EQ(-1, uvec[2]);

    const int data_size = uvec.size() * sizeof(decltype(uvec)::value_type);

    std::string storage;

    {  // Serialize

        // ostream
        // https://en.cppreference.com/w/cpp/io/basic_ostream
        // Unformatted output:
        //    put: inserts a character
        //    write: inserts blocks of characters
        // Behaves as an UnformattedOutputFunction. After constructing and checking the sentry object, outputs the
        // characters from successive locations in the character array whose first element is pointed to by s.
        // Characters are inserted into the output sequence
        std::ostringstream out;
        out.write(reinterpret_cast<char*>(uvec.data()), data_size);
        storage = out.str();
    }

    {
        //
        // https://en.cppreference.com/w/cpp/io/basic_istream/read
        // Unformatted input
        //  get
        //  peek
        //  unget
        //  putback
        //  getline
        //  ignore
        //  read: extracts blocks of characters
        //  readsome
        //  gcount

        /*
        basic_istream& read( char_type* s, std::streamsize count );

        Behaves as UnformattedInputFunction. After constructing and checking the sentry object, extracts characters and
        stores them into successive locations of the character array whose first element is pointed to by s. Characters
        are extracted and stored
        */
        // std::istream input_stream;
        std::istringstream input_stream(storage);
        // Extracts characters from stream.
        std::vector<short> devec;
        devec.resize(uvec.size());
        if (!input_stream.read(reinterpret_cast<char*>(devec.data()), data_size)) {
            FAIL() << "Failed to read the vector into istream";
        }
        EXPECT_EQ(3, devec.size());
        EXPECT_EQ(-32768, devec[0]);
        EXPECT_EQ(32767, devec[1]);
        EXPECT_EQ(-1, devec[2]);
    }
}
