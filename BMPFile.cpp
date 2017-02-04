/**
 * Defines the object structure for BMP files. Handles the reading of a bmp file from a file name, and storing the
 * appropriate data as it is received from the header and bytes of the file.
 *
 * Reading of byte data routine taken from: http://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
 *
 * @author Sasha Ouellet
 * @version 1.0 - 01-25-17
 */

#include "BMPFile.h"

/**
 * Constructor for the BMPFile, given a specific file name to read. Will populate the pixelData array with the
 * R, G, B values of each pixel. This array's length is equal to 3 * width * height of the image (as found in the BMP
 * header)
 *
 * @param fileName The char array (string) containing the name of the BMP file to read
 */
BMPFile::BMPFile(const char* fileName)
{
	FILE* f = fopen(fileName, "rb");
	unsigned char info[54];

	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;
	cout << "Name:   \t" << fileName << endl;
	cout << "Width:  \t" << width << endl;
	cout << "Height: \t" << height << endl;

	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel

	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	// Flipping R and B values since they are stored backwards in BMPs
	for (int i = 0; i < size; i += 3)
	{
		unsigned char tmp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tmp;
	}

	flipY();

	m_fileName = fileName;
	m_pixelData = data;
	m_width = width;
	m_height = height;

	delete [] data;
}

/**
 * Gets the pixel data array for this BMP
 * @return The pixel data array of the BMP
 */
unsigned char* BMPFile::getPixels()
{
	return m_pixelData;
}

/**
 * Dumps out each pixel's R, G, B value to system out.
 *
 * NOTE: Due to how the BMP file protocol works, the data is stored bottom-up, so this information appears "reversed"
 */
void BMPFile::printPixelData()
{
	for (int j = 0; j < m_height * 3; j += 3)
	{
		for (int i = 0; i < m_width * 3; i += 3)
		{
			cout << "R: " << static_cast<int>(m_pixelData[j * m_width + i]) << " G: " << static_cast<int>(m_pixelData[j * m_width + i + 1]) << " B: " << static_cast<int>(m_pixelData[j * m_width + i + 2]) << endl;
		}
	}
}

/**
 * Gets the pixel data at the given x, y value
 * @param x The x value of the pixel wanted
 * @param y The y value of the pixel wanted
 * @return A length 3 vector containing the R, G, B values of the desired pixel at x, y
 */
vector<unsigned int> BMPFile::getPixel(int x, int y)
{
	if (x >= m_width || y >= m_height)
	{
		throw invalid_argument("Received x or y value that exceeds width or height of image");
	}

	vector<unsigned int> data;

	x *= 3;

	y = m_height - 1 - y; // Since BMP is stored bottom-up so we need to flip it
	y *= 3;

	unsigned int r = static_cast<unsigned int>(m_pixelData[y * m_width + x]);
	unsigned int g = static_cast<unsigned int>(m_pixelData[y * m_width + x + 1]);
	unsigned int b = static_cast<unsigned int>(m_pixelData[y * m_width + x + 2]);

	data.push_back(r);
	data.push_back(g);
	data.push_back(b);

	return data;

}

/**
 * Writes the given pixel data array as a BMP, with the width and height provided used to determine the file size, and
 * for header information
 * @param width The width of the image
 * @param height The height of the image
 * @param pixelData The pixel array of R, G, B values. Assumes that the R and B values have not been switched, and that
 *        the array is still stored bottom-up
 * @param name The name of the file to save to (should include .bmp, i.e. "image.bmp")
 */
void BMPFile::writeFile(int width, int height, unsigned char* pixelData, const char* name)
{
	int fileSize = 54 + 3 * width * height;

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2]  = (unsigned char)(fileSize);
	bmpfileheader[3]  = (unsigned char)(fileSize >> 8);
	bmpfileheader[4]  = (unsigned char)(fileSize >> 16);
	bmpfileheader[5]  = (unsigned char)(fileSize >> 24);

	bmpinfoheader[4]  = (unsigned char)(width);
	bmpinfoheader[5]  = (unsigned char)(width >> 8);
	bmpinfoheader[6]  = (unsigned char)(width >> 16);
	bmpinfoheader[7]  = (unsigned char)(width >> 24);
	bmpinfoheader[8]  = (unsigned char)(height);
	bmpinfoheader[9]  = (unsigned char)(height >> 8);
	bmpinfoheader[10] = (unsigned char)(height >> 16);
	bmpinfoheader[11] = (unsigned char)(height >> 24);

	int size = 3 * width * height;

	for (int i = 0; i < size; i += 3) // Flip R and B back
	{
		unsigned char tmp = pixelData[i];
		pixelData[i] = pixelData[i + 2];
		pixelData[i + 2] = tmp;
	}

	FILE *f;
	f = fopen(name, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (int i = 0; i < height; i++)
	{
		fwrite(pixelData + (width * (i) * 3), 3, width, f);
		fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, f);
	}
	fclose(f);
}

/**
 * Flips the data in the pixel array so that the data can be read top-down instead of bottom-up
 */
void BMPFile::flipY()
{
	const int rowSize = m_width * 3;
	unsigned char* flipped = new unsigned char[rowSize * m_height];

	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0 ; j < m_width ; j++)
		{
			cout << "i: " << i << ", j: " << j << endl;
			flipped[3 * (i * m_width + j) + 0] = m_pixelData[3 * (i * m_width + j) + 0];
			flipped[3 * (i * m_width + j) + 1] = m_pixelData[3 * (i * m_width + j) + 1];
			flipped[3 * (i * m_width + j) + 2] = m_pixelData[3 * (i * m_width + j) + 2];
		}
	}

	m_pixelData = flipped;

	delete [] flipped;
}
