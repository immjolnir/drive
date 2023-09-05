// https://www.cfnotes.com/archives/305
/*
在C++中，使用std::cout输出一些内容，需要每一行的某些内容能够对齐，并且占的宽度一致，比如输出一个类似表格的内容时，想要每一列都对齐，可以使用std::cout的格式控制来实现。
比如，以下代码中，需要将myHumen输出为一个三行四列，并且每列能对齐的内容。
*/
#include <iomanip>
#include <iostream>
using namespace std;

struct Human {
    char name[20];
    char name2[20];
    char name3[20];
    double pts;
};

int main() {
    int i;
    Human myHumen[3] = {
      {"Mr", "Alan", "Turing", 12.25}, {"Ms", "Ada", "Lovelace", 15.25}, {"Sir", "Edgar Allan", "Poe", 45.25}};
    cout << "Name1"
         << "Name2"
         << "Name3"
         << "Rating" << endl;
    cout << "-----------------------------------------------------------------------\n";
    for (i = 0; i < 3; i++) {
        cout << myHumen[i].name << myHumen[i].name2 << myHumen[i].name3 << myHumen[i].pts << endl;
    }  // this didn't do nice printouts, with leftalign for text and rightalign with numbers

    // std::cout的内容宽度和对齐配置，可以用如下方式实现：
    cout << std::left << setw(8) << "Name1" << std::left << setw(12) << "Name2" << std::left << setw(10) << "Name3"
         << std::left << setw(10) << "Rating" << endl;
    cout << "-----------------------------------------------------------------------\n";
    for (i = 0; i < 3; i++) {
        cout << std::left << setw(8) << myHumen[i].name << std::left << setw(22) << myHumen[i].name2 << std::left
             << setw(22) << myHumen[i].name3 << std::left << setw(20) << myHumen[i].pts << endl;
    }  // this didn't do nice printouts, with leftalign for text and rightalign with numbers
}