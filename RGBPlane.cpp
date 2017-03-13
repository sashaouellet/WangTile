/**
 * The RGBPlane class represents an XY plane of RGB values for a pixel plane.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/25/17
 */

#include <stdexcept>
#include <iostream>
#include <cmath>
#include "RGBPlane.h"

using namespace std;

/**
 * Constructs the RGBPlane with the specified dimensions
 *
 * @param width The width of the plane
 * @param height The height of the plane
 */
RGBPlane::RGBPlane(int width, int height)
{
    m_width = width;
    m_height = height;
    m_pixelData = new unsigned char[m_width * m_height * 3]; // Since we are holding 3 values for every pixel
}

/**
 * Copy constructor
 *
 * @param plane The plane to copy the pixel values from
 */
RGBPlane::RGBPlane(const RGBPlane& plane)
{
    m_width = plane.m_width;
    m_height = plane.m_height;
    m_pixelData = new unsigned char[plane.m_width * plane.m_height * 3];

    copy(plane.m_pixelData, plane.m_pixelData + (plane.m_width * plane.m_height * 3), m_pixelData);
}

RGBPlane::~RGBPlane()
{
//    delete [] m_pixelData;
}

/**
 * Gets the index of the start of the RGB values for the given pixel at x, y
 *
 * @param x The x value of the point
 * @param y The y value of the point
 * @return The start index of the RGB values in the pixel data array
 */
int RGBPlane::getIndexFromPoint(int x, int y)
{
    x *= 3;
    y *= 3;

    return y * m_width + x;
}

/**
 * Gets the RGB values for the given pixel at x, y
 *
 * @param x The x value of the pixel in the plane
 * @param y The y value of the pixel in the plane
 * @param flip If the y values should be flipped to accommodate retrieving data from a bitmap structure
 * @return A size 3 vector containing the RGB values of the specified pixel
 * @throws invalid_argument if the given x or y values exceeds the width or height of the plane (or less than 0)
 */
vector<unsigned char> RGBPlane::getPixelValueAt(int x, int y, bool flip)
{
    y = flip ? m_height - 1 - y : y;

    if (x >= m_width || y >= m_height || x < 0 || y < 0)
    {
        throw invalid_argument("Received x or y value that exceeds width or height of plane (or they are less than 0)");
    }

    vector<unsigned char> data(3);
    int startIndex = getIndexFromPoint(x, y);

    data[0] = m_pixelData[startIndex];
    data[1] = m_pixelData[startIndex + 1];
    data[2] = m_pixelData[startIndex + 2];

    return data;
}

/**
 * Sets the RGB values for the pixel at x, y with the RGB values specified by r, g, and b.
 *
 * @param x The x value of the pixel to set
 * @param y The y value of the pixel to set
 * @param r The red value for the pixel
 * @param g The green value for the pixel
 * @param b The blue value for the pixel
 * @param flip If the y values should be flipped to accommodate retrieving data from a bitmap structure
 * @throws invalid_argument if the given x or y values exceeds the width or height of the plane (or less than 0)
 */
void RGBPlane::setPixelValueAt(int x, int y, unsigned char r, unsigned char g, unsigned char b, bool flip)
{
    y = flip ? m_height - 1 - y : y;

    if (x >= m_width || y >= m_height || x < 0 || y < 0)
    {
        throw invalid_argument("Received x or y value that exceeds width or height of plane (or they are less than 0)");
    }

    int startIndex = getIndexFromPoint(x, y);

    m_pixelData[startIndex] = r;
    m_pixelData[startIndex + 1] = g;
    m_pixelData[startIndex + 2] = b;
}

/**
 * Gets the raw value at a specific index
 *
 * @param ind The index to get the data at
 * @return The data at the specified index
 * @throws invalid_argument If the given index is out of bounds for this plane's allocated data
 */
unsigned char RGBPlane::getValueAt(int ind)
{
    if (ind >= (m_width * m_height * 3))
    {
        throw invalid_argument("Index out of bounds of stored data for this plane");
    }

    return m_pixelData[ind];
}

/**
 * Gets the specified pixel region of this plane within the 2 corners provided
 * @param x1 The top left corner x-value
 * @param y1 The top left corner y-value
 * @param x2 The bottom right corner x-value
 * @param y2 The bottom right corner y-value
 * @param flip If the y values should be flipped to accommodate retrieving data from a bitmap structure
 * @return A new subset region plane from the given region
 */
RGBPlane* RGBPlane::getRegion(int x1, int y1, int x2, int y2, bool flip)
{
    int width = x2 - x1 + 1;
    int height = y2 - y1 + 1;
    RGBPlane* region = new RGBPlane(width, height);

    for (int i = y1, y = 0 ; i <= y2 ; i++, y++)
    {
        for (int j = x1, x = 0 ; j <= x2 ; j++, x++)
        {
            vector<unsigned char> data = getPixelValueAt(j, i, flip);

            region->setPixelValueAt(x, y, data[0], data[1], data[2], flip);
        }
    }

    return region;
}

/**
 * Flips the red and blue values for the entire pixel plane.
 *
 * This is needed for bitmap files since they are stored in
 * the bitmap format as BGR, but when they are read in we flip them back to RGB for simplicity's sake. They must be
 * flipped again when re-exporting the pixel data to a bitmap.
 */
void RGBPlane::flipRBValues()
{
    int size = m_width * m_height * 3;

    for (int i = 0; i < size; i += 3)
    {
        unsigned char tmp = m_pixelData[i];
        m_pixelData[i] = m_pixelData[i + 2];
        m_pixelData[i + 2] = tmp;
    }
}

/**
 * Resizes the plane to the specified dimensions. This deletes the old pointer to the pixel array and creates a new
 * one.
 *
 * @param width The new width of the plane
 * @param height The new height of the plane
 */
void RGBPlane::setDimensions(int width, int height)
{
    m_width = width;
    m_height = height;

    delete [] m_pixelData;

    m_pixelData = new unsigned char[m_width * m_height * 3];
}

int RGBPlane::getWidth() const
{
    return m_width;
}

int RGBPlane::getHeight() const
{
    return m_height;
}

/**
 * Gets the actual data from the structure.
 * @return The actual pointer to the data (NOT A COPY). Whoever calls this must not delete this pointer.
 */
unsigned char* RGBPlane::getRawData()
{
    return m_pixelData;
}

/**
 * Rotates the plane 45 degrees, effectively resizing to account for the increase in the bouding box size and remapping all
 * the pixel data
 */
RGBPlane* RGBPlane::rotate()
{
    float angle = -45 * (3.141592/180.0f);
    float midX = ((float)m_width) / 2.0f;
    float midY = ((float)m_height) / 2.0f;

    float sine = sin(angle);
    float cosine = cos(angle);

    // x = x cos - y sin
    float topLeftTransformedX = (-midX * cosine) - (midY * sine);
    float topRightTransformedX = (midX * cosine) - (midY * sine);
    float bottomLeftTransformedX  = (-midX * cosine) - (-midY * sine);
    float bottomRightTransformedX = (midX * cosine) - (-midY * sine);

    float minx = min(topLeftTransformedX, min(topRightTransformedX, min(bottomLeftTransformedX, bottomRightTransformedX)));
    float maxx = max(topLeftTransformedX, max(topRightTransformedX, max(bottomLeftTransformedX, bottomRightTransformedX)));

    // y = x sin + y cos
    float topLeftTransformedY = (-midX * sine) + (midY * cosine);
    float topRightTransformedY = (midX * sine) + (midY * cosine);
    float bottomLeftTransformedY  = (-midX * sine) + (-midY * cosine);
    float bottomRightTransformedY = (midX * sine) + (-midY * cosine);

    float miny = min(topLeftTransformedY, min(topRightTransformedY, min(bottomLeftTransformedY, bottomRightTransformedY)));
    float maxy = max(topLeftTransformedY, max(topRightTransformedY, max(bottomLeftTransformedY, bottomRightTransformedY)));

    int outWidth = (int) ceil(fabs(maxx) + fabs(minx));
    int outHeight = (int) ceil(fabs(maxy) + fabs(miny));

    RGBPlane* newPlane = new RGBPlane(outWidth, outHeight);

    for (int x = 0; x < outWidth; x++)
    {
        for (int y = 0; y < outHeight; y++)
        {
            float error = 0.858; // I'm not sure why I need this but the transform is slightly off without it
            float srcX = ((cosine * (x - midX)) - (sine * (y - midY))) + (midX / 2) * error;
            float srcY = ((sine * (x - midX)) + (cosine * (y - midY))) + midY;

            if (srcX >= 0 && srcX < m_width && srcY >= 0 && srcY < m_height)
            {
                vector<unsigned char> pixel = getPixelValueAt(srcX, srcY, true);

                newPlane->setPixelValueAt(x, y, pixel[0], pixel[1], pixel[2], true);
            }
        }
    }

    return newPlane;
}
