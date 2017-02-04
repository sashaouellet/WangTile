#include <iostream>
#include "BMPFile.h"

using namespace std;

int main() {
    BMPFile bmp("test.bmp");

	vector<unsigned int> pixel = bmp.getPixel(299, 287);

//    cout << pixel[0] << ", " << pixel[1] << ", " << pixel[2];

	BMPFile::writeFile(300, 288, bmp.getPixels(), "test2.bmp");

    return 0;
}

