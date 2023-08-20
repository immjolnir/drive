#include <iomanip>
#include <iostream>

using namespace std;

int main(void) {
    double h = 5.7000;
    // C++默认是不输出浮点数小数点后多余的0的。如果想要输出小数点后多余的0，则要在程序中用cout.setf(ios::showpoint);语句设置，不再想输出时要用cout.unsetf(ios::showpoint);语句恢复。以下代码供理解这个变化过程：
    // cout << std::fixed;
    cout << setprecision(6);
    cout << "默认不输出小数点后的0\n" << h << endl;
    cout.setf(ios::showpoint);  // 设置输出小数点后多余的0
    cout << h << endl;
    cout << 0 << endl;
    cout.unsetf(ios::showpoint);  // 恢复默认设置,即不再输出多余的0
    cout << h << endl;
    cout << 0 << endl;

    {
        int n = 141;
        // 1) 分别以十六进制、十进制、八进制先后输出 n
        cout << "1)" << hex << n << " " << dec << n << " " << oct << n << endl;
        double x = 1234567.89, y = 12.34567;
        // 2)保留5位有效数字
        cout << "2)" << setprecision(5) << x << " " << y << " " << endl;
        // 3)保留小数点后面5位
        cout << "3)" << fixed << setprecision(5) << x << " " << y << endl;
        // 4)科学计数法输出，且保留小数点后面5位
        cout << "4)" << scientific << setprecision(5) << x << " " << y << endl;
        // 5)非负数显示正号，输出宽度为12字符，宽度不足则用 * 填补
        cout << "5)" << showpos << fixed << setw(12) << setfill('*') << 12.1 << endl;
        // 6)非负数不显示正号，输出宽度为12字符，宽度不足则右边用填充字符填充
        cout << "6)" << noshowpos << setw(12) << left << 12.1 << endl;
        // 7)输出宽度为 12 字符，宽度不足则左边用填充字符填充
        cout << "7)" << setw(12) << right << 12.1 << endl;
        // 8)宽度不足时，负号和数值分列左右，中间用填充字符填充
        cout << "8)" << setw(12) << internal << -12.1 << endl;
        cout << "9)" << 12.1 << endl;
    }
    return 0;
}

/**
当使用 fixed 时，它输出个小数固定是6位。即便是0, 输出也是0.000000

$ head -1 bumper_radar_radar_tracks.txt.old
1681726492707 0
127.800003,-4.600000,0.000000;127.800003,-4.600000,0.000000;127.800003,-4.600000,0.000000;127.800003,-4.600000,0.000000
-19.250000 -0.500000 0.000000

当setprecision(6)时，只有小数的位数大于6位时，才有效果。所以输出0时，只输出0而已。

$ head -1 bumper_radar_radar_tracks.txt
1681726492707 0 127.8,-4.6,0;127.8,-4.6,0;127.8,-4.6,0;127.8,-4.6,0 -19.25 -0.5 0
 */
