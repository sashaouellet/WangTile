#include <iostream>
#include "BMPFile.h"

using namespace std;

int main() {
    BMPFile bmp("test.bmp");

//	bmp.printPixelData();

	vector<unsigned int> pixel = bmp.getPixel(0, 0);

	bmp.writeFile();

    return 0;
}

