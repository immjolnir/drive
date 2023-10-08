#include <iostream>

using namespace std;

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

int main() {
    my_array arr(2, 1);

    {
        my_array tmp(3, 2);
        arr = tmp;
    }
    std::cout << "size=" << arr.size << std::endl;

    return 0;
}