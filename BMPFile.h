/**
 * Defines the object structure for BMP files. Handles the reading of a bmp file from a file name, and storing the
 * appropriate data as it is received from the header and bytes of the file.
 *
 * Reading of byte data routine taken from: http://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 01-25-17
 */

#ifndef WANGTILE_BMPFILE_H
#define WANGTILE_BMPFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

using namespace std;

class BMPFile
{
private:
    const char* m_fileName;
    unsigned char* m_pixelData;
	int m_width;
    int m_height;
public:
	BMPFile(const char*);
    unsigned char* getPixels();
    const char* getFileName();
	void printPixelData();
	vector<unsigned int> getPixel(int, int);
	static void writeFile(int, int, unsigned char*, const char*);
    int getWidth();
    int getHeight();
    unsigned char* getPixelRegion(unsigned int, unsigned int, unsigned int, unsigned int);

    virtual ~BMPFile();
};

#endif //WANGTILE_BMPFILE_H
