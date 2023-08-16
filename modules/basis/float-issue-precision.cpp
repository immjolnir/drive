// 
// https://blog.miigon.net/posts/cpp-ios-base-fixed-precision-loss/
//
#include <iostream>
#include <iomanip>

int main() {
    using namespace std;
    
    std::ios_base::fmtflags old_f(cout.flags());
    cout << "Returns current formatting settings: " << cout.flags() << endl;

    // https://stackoverflow.com/questions/2273330/restore-the-state-of-stdcout-after-manipulating-it
    float result = (55.25f+11.17f);

    cout << "default: "<< result << endl;    // 66.419998

    cout.precision(6);  // 6 位有效数字
    cout << result << endl;    // 66.42
    cout.precision(8);  // 8 位有效数字
    cout << result << endl;    // 66.419998
    
    cout.setf(ios_base::fixed,ios_base::floatfield);
    cout.precision(6);  // 小数点后 6 位
    cout << result << endl;    // 66.419998

    // https://en.cppreference.com/w/cpp/io/ios_base
    // 1. Clearing all error state flags
    // cout.clear();

    // 2. Clearing the ios formatting flags type
    cout << "Returns current formatting settings: " << cout.flags() << endl;
    cout.flags(old_f);
    cout <<"restore to default: "<< result << endl;    // 66.419998

    cout << std::fixed << result << endl; // 默认是6位
    cout << std::setprecision(16) << result << endl; // 默认是6位
    return 0;
}
