#include <limits>
#include <iostream>
#include <typeinfo>

#define stringfy(T) std::string(#T)

#define printTypeInfo(T) printTypeInfoImpl<T>(#T)

template<typename T>
void printTypeInfoImpl(const std::string& real_type_name) {
    // https://en.cppreference.com/w/cpp/types/numeric_limits
    // https://en.cppreference.com/w/cpp/types/type_info/name
    // https://stackoverflow.com/questions/4484982/how-to-convert-typename-t-to-string-in-c
        static const std::string name = typeid(T).name();

        T min = std::numeric_limits<T>::min();
        T max = std::numeric_limits<T>::max();
        // digits: number of radix digits that can be represented without change
        // digits10: number of decimal digits that can be represented without change
        size_t bits_count = std::numeric_limits<T>::digits;
        int radix = std::numeric_limits<T>::radix;

        std::cout << stringfy(T) << "] sizeof(" << name << ":" << real_type_name << ")=" << sizeof(T) << ", min=" << min << ", max=" << max 
            << ", radix=" << radix
            << ", bits=" << bits_count << std::endl;
}

int main() {
    printTypeInfo(long);
    printTypeInfo(unsigned long);

    printTypeInfo(long long);
    printTypeInfo(unsigned long long);

    printTypeInfo(float);
    printTypeInfo(double);
    printTypeInfo(long double);

}
