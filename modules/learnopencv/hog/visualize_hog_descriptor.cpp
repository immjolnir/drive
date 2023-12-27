#include "HOGImage.h"

using namespace cv;
using namespace std;

// error: unused parameter 'argc' [-Werror=unused-parameter]
int main(int argc, const char* argv[]) {
    string tearser_image_file(argv[1]);
    Mat image = imread(tearser_image_file);
    HOGDescriptor hogDesc(image.size(), Size(20, 20), Size(10, 10), Size(10, 10), 9);

    Mat hogImg = HOGImage(image, hogDesc, 3, 2);

    string name = "HOG Image window";
    namedWindow(name);
    imshow(name, hogImg);
    waitKey();
    imwrite("hogImg.jpg", hogImg);

    return 0;
}
