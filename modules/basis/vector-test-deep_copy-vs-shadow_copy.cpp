/**
 * https://stackoverflow.com/questions/11348376/std-vector-c-deep-or-shallow-copy
 *
 * Vector will resize to have enough space for the objects. It will then iterate through the objects and call the
 * default copy operator for every object. In this way, the copy of the vector is 'deep'. The copy of each object in the
 * vector is whatever is defined for the default copy operator.
 */

#include <iostream>
#include <vector>

#include <gtest/gtest.h>

using namespace std;

namespace shadow_copy
{  // default

    class my_array {
      public:
        int* array;
        int size;

        my_array(int size, int init_val) : size(size) {
            array = new int[size];
            for (int i = 0; i < size; ++i) {
                array[i] = init_val;
            }
        }

        ~my_array() {
            cout << "Destructed " << array[0] << endl;
            if (array != NULL) {
                delete[] array;
            }
            array = NULL;
            size = 0;
        }
    };
}  // namespace shadow_copy

namespace deep_copy
{
    class my_array {
      public:
        int* array;
        int size;

        my_array(int size, int init_val) : size(size) {
            cout << "contsructed " << init_val << endl;
            array = new int[size];
            for (int i = 0; i < size; ++i) array[i] = init_val;
        }

        // Providing the copy construtor to perform deep copy
        my_array(const my_array& to_copy) {
            cout << "deep copied " << to_copy.array[0] << endl;
            array = new int[to_copy.size];
            size = to_copy.size;
            for (int i = 0; i < to_copy.size; i++) {
                array[i] = to_copy.array[i];
            }
        }

        ~my_array() {
            cout << "Destructed " << array[0] << endl;
            if (array != NULL) {
                delete[] array;
            }
            array = NULL;
            size = 0;
        }
    };
}  // namespace deep_copy

TEST(copy_existing_obj, shadow_copy) {
    // How to skip a test in gtest
    // https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test
    //
    GTEST_SKIP() << "Skipping it for its crash issue.";

    try {
        vector<shadow_copy::my_array> c;
        {
            shadow_copy::my_array a(5, 0);
            c.emplace_back(a);
        }
        c.emplace_back(shadow_copy::my_array(4, 1));

        // At this point the destructor of c[0] and c[1] has been called.
        // However vector still holds their 'remains'
        cout << c[0].size << endl;      // should be fine, as it copies over with the = operator
        cout << c[0].array[0] << endl;  // undefined behavior, the pointer will get copied, but the data is not valid
    } catch (const std::exception& e) {
        std::cerr << "base handler] " << e.what() << '\n';  // or whatever
        // EXPECT_EQ(e.what(), std::string("Base"));
    }
}

/*
How to test a program with crash?
https://stackoverflow.com/questions/6569713/testing-for-crash-with-google-test
You can use a death test to isolate a crash:

EXPECT_EXIT(ImNotNiceToPointers(p); exit(0), ExitedWithCode(0), '');

However, I recommend against using death tests. Using a death test incurs the overhead of launching a subprocess whether
there is a crash or not. But if you simply leave your code as is and your test crashes, you will know, and will be able
to fix it.

Tracing the origin of the crash is easy with the help of tools like Valgrind or Dr. Memory.
*/

TEST(copy_existing_obj, deep_copy) {
    vector<deep_copy::my_array> c;
    {
        deep_copy::my_array a(5, 0);
        c.emplace_back(a);
    }
    c.emplace_back(deep_copy::my_array(4, 1));

    // At this point the destructor of c[0] and c[1] has been called.
    // However vector holds a deep copy'
    cout << c[0].size << endl;      // This is FINE
    cout << c[0].array[0] << endl;  // This is FINE
}