/**
 * The IntPlane class represents an XY plane of pixels, and the integer value each of these pixels holds.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/25/17
 */

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "IntPlane.h"

using namespace std;

/**
 * Construct with the given dimensions of the plane. Will initialize the array of the pixel data values according to
 * this size and will keep the pointer to this array. The IntPlane is responsible for deleting the memory at this pointer
 *
 * @param width The width of the plane
 * @param height The height of the plane
 */
IntPlane::IntPlane(int width, int height)
{
    m_width = width;
    m_height = height;
    m_pixelData = new int[width * height];
}

/**
 * Copy constructor for the IntPlane
 *
 * @param plane The reference to the IntPlane this new IntPlane will be copied from
 */
IntPlane::IntPlane(const IntPlane& plane)
{
    m_width = plane.m_width;
    m_height = plane.m_height;
    m_pixelData = new int[m_width * m_height];

    copy(plane.m_pixelData, plane.m_pixelData + (plane.m_width * plane.m_height), m_pixelData);
}

IntPlane::~IntPlane()
{
    delete [] m_pixelData;
}

/**
 * Gets the index in the int array that the associated xy-coords lie at
 *
 * @param x The x value of the point
 * @param y The y value of the point
 * @return The index of the specified point in the array
 */
int IntPlane::getIndexFromPoint(int x, int y)
{
    return (y * m_width + x);
}

/**
 * Gets the integer value stored at the given pixel position in the XY plane
 *
 * @param x The x value of the pixel in the plane
 * @param y The y value of the pixel in the plane
 * @return The integer stored at this position
 * @throws invalid_argument if the given x or y values exceeds the width or height of the plane (or less than 0)
 */
int IntPlane::getPixelValueAt(int x, int y)
{
    if (x >= m_width || y >= m_height || x < 0 || y < 0)
    {
        throw invalid_argument("Received x or y value that exceeds width or height of plane (or they are less than 0)");
    }

    return m_pixelData[getIndexFromPoint(x, y)];
}

/**
 * Sets the integer value of the pixel at the specified xy-coords
 *
 * @param x The x value of the pixel in the plane
 * @param y The y value of the pixel in the plane
 * @param value The value to set the pixel information to
 * @throws invalid_argument if the given x or y values exceeds the width or height of the plane (or less than 0)
 */
void IntPlane::setPixelValueAt(int x, int y, int value)
{
    if (x >= m_width || y >= m_height || x < 0 || y < 0)
    {
        throw invalid_argument("Received x or y value that exceeds width or height of plane (or they are less than 0)");
    }

    m_pixelData[getIndexFromPoint(x, y)] = value;
}

/**
 * Fills the entire plane with the specified value
 * @param value The integer value to assign to every spot in the plane
 */
void IntPlane::fill(int value)
{
	std::fill(m_pixelData, m_pixelData + (m_width * m_height), value);
}

/**
 * For debug purposes
 */
void IntPlane::print()
{
	for (int i = 0 ; i < m_height ; i++)
	{
		for (int j = 0 ; j < m_width ; j++)
		{
			cout << getPixelValueAt(j, i) << " ";
		}
		cout << endl;
	}
}
