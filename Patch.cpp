/**
 * The Patch class defines a square piece of a large input image that will then be "quilted" together along with other
 * patches in order to synthesize a larger overall output texture.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/13/17
 */

#include "Patch.h"
#include "Quilt.h"

/**
 * The default constructor for the Patch. Uses the given unsigned char array as its pixel data map.
 * @param data The pixel data for this particular patch. This is derived from a pre-determined square dimension segment
 *             of the original input image
 * @param dimension The length of the Patch edges
 */
Patch::Patch(const RGBPlane& plane, unsigned int dimension)
{
    m_pixelData = new RGBPlane(plane);
    m_dimension = dimension;
    m_error = new IntPlane(dimension, dimension);
	m_boundaries = new IntPlane(dimension, dimension);
    m_totalError = 0;
	m_cornerCutX = 0;
	m_cornerCutY = 0;
}

Patch::Patch(const Patch &patch)
{
    m_pixelData = new RGBPlane(*patch.getRGBPlane());
    m_dimension = patch.m_dimension;
    m_error = new IntPlane(*patch.getErrorPlane());
	m_boundaries = new IntPlane(*patch.getBoundaries());
    m_totalError = 0;
	m_cornerCutX = patch.m_cornerCutX;
	m_cornerCutY = patch.m_cornerCutY;
}

Patch::~Patch()
{
    delete m_pixelData;
    delete m_error;
	delete m_boundaries;
}

/**
 * Gets the pixel data plane of this patch
 * @return The pixel data plane
 */
RGBPlane* Patch::getRGBPlane() const
{
    return m_pixelData;
}

/**
 * Gets the error data plane of this patch
 * @return The error data plane
 */
IntPlane* Patch::getErrorPlane() const
{
    return m_error;
}

/**
 * Gets the boundary of the Patch, where the cuts will be made. 0 on the plane means
 * the pixel is untouched. 1 signfies that the cut travels through that pixel.
 * @return The boundaries plane
 */
IntPlane* Patch::getBoundaries() const
{
	return m_boundaries;
}

/**
 * Populates the error 2-dimensional array by calculating the overlap score given the 2 patches. Score is determined
 * by the L2 norm of the difference in pixel values for each channel (r, g, b) of the overlapping patches.
 *
 * e.g. sqrt((thisPatch.r - otherPatch.r)^2 + (thisPatch.g - otherPatch.g)^2 + (thisPatch.b - otherPatch.b)^2)
 *
 * For every pixel of overlap
 *
 * @param left The patch to the left of this one, nullptr if this is the leftmost patch in the row
 * @param top The patch above this patch, nullptr if this is the topmost row
 * @return The total error of the overlap region
 */
unsigned int Patch::getOverlapScore(Patch *left, Patch *top)
{
    int overlap = m_dimension / Quilt::OVERLAP_DIVISOR;

    for (int i = 0 ; i < m_dimension ; i++)
    {
        for (int j = 0 ; j < m_dimension ; j++)
        {
            // If top overlap region and has a patch above it
            if (i < overlap && top != nullptr)
            {
                int error = util::l2NormDiff(getPixelAt(j, i), top->getPixelAt(j, m_dimension - overlap + i), 3);
                m_error->setPixelValueAt(j, i, error);
                m_totalError += error;
            }
            // if left overlap region and has patch to the left of it
            else if (j < overlap && left != nullptr)
            {
                int error = util::l2NormDiff(getPixelAt(j, i), left->getPixelAt(m_dimension - overlap + j, i), 3);
                m_error->setPixelValueAt(j, i, error);
                m_totalError += error;
            }
        }
    }
    return m_totalError;
}

/**
 * Gets the pixel data from the given x, y coords.
 * @param x The x coord of the pixel
 * @param y The y coord of the pixel
 * @return Array containing the r, g, b values of the desired pixel
 */
int* Patch::getPixelAt(int x, int y)
{
    if (x >= m_dimension || y >= m_dimension)
    {
        throw invalid_argument("Received x or y value that exceeds width or height of patch");
    }

    int* data = new int[3];
    vector<unsigned char> values = m_pixelData->getPixelValueAt(x, y, false);

    data[0] = static_cast<int>(values[0]);
    data[1] = static_cast<int>(values[1]);
    data[2] = static_cast<int>(values[2]);

    return data;
}

/**
 * Gets the total error of the calculated overlap region for this patch
 * @return The total summed error of the overlap region
 * @see Patch::getOverlapScore(Patch*, Patch*)
 */
unsigned int Patch::getTotalError()
{
    return m_totalError;
}

/**
 * Gets the side length of the patch
 * @return The side length (pixels) of the patch
 */
unsigned int Patch::getDimension()
{
    return m_dimension;
}

/**
 * Calculates the cut that needs to be made through this patch's pixels in order to produce the smallest margin of error
 * when quilting it in relation to the provided left and top patches
 * @param left The left patch, nullptr if this is the leftmost in the row
 * @param right The right patch, nullptr if this is the rightmost in the row
 * @param top The patch above this one, nullptr if this is the first row
 * @param bottom The patch below this one, nullptr if this is the last row
 */
void Patch::calculateLeastCostBoundaries(Patch* left, Patch* right, Patch* top, Patch* bottom, bool print)
{
	m_boundaries->fill(0);

	cutTopBoundary(left, right);
	cutLeftBoundary(top, bottom);

	m_error->print();
	cout << endl;
	m_boundaries->print();
}

/**
 * Makes the cut along the top overlap region, given the Patches to the left and right of this one.
 *
 * @param left The patch to the left, nullptr if this is the leftmost Patch in the row. We will cut all the way to
 * 		        the end if this is the case. Otherwise, we will create a corner
 * @param right The patch to the right, nullptr if this is the rightmost Patch in the row. We use this patch to determine
 * 				the starting point corner of where the seam on this Patch begins
 */
void Patch::cutTopBoundary(Patch* left, Patch* right)
{
	int overlap = m_dimension / Quilt::OVERLAP_DIVISOR;
	int startX = m_dimension - 1;

	int previousRow = -1; // We keep track as we move in the X direction where (in Y) the previous column was cut at
	// since we can only go within -1 -> +1 from that for the next cut

	if (right != nullptr)
	{
		startX = m_dimension - overlap + right->getCornerCut()[0] - 1;
		previousRow = right->getCornerCut()[1];
	}

	int endX = -1;

	if (left != nullptr)
	{
		determineCorner();
		endX = getCornerCut()[0];
	}

	for (int x = startX; x >= endX + 1; x--)
	{
		if (x == m_dimension - 1 && right != nullptr) // Haven't determined a starting cut yet
		{
			int smallest = m_error->getPixelValueAt(x, 0);
			int index = 0;

			for (int i = 1; i < overlap; i++) // So we have to iterate the entire column to find the smallest error
			{
				int error = m_error->getPixelValueAt(x, i);

				if (error < smallest)
				{
					smallest = error;
					index = i;
				}
			}
			previousRow = index;
			m_boundaries->setPixelValueAt(x, previousRow, 1);
		}
		else // Now we can only iterate from -1 to +1 of previousRow
		{
			int start = max(0, previousRow - 1);
			int end = min(overlap, previousRow + 1);
			int smallest = INT_MAX;
			int index = 0;

			for (int i = start; i < end; i++)
			{
				int error = m_error->getPixelValueAt(x, i);

				if (error < smallest)
				{
					smallest = error;
					index = i;
				}
			}
			previousRow = index;
			m_boundaries->setPixelValueAt(x, previousRow, 1);
		}
	}
}

/**
 * Makes the cut along the left overlap region, given the Patches above and below this one.
 *
 * @param top The patch above, determines if we should cut all the way to the top of this Patch, or stop at a corner
 * @param bottom The patch below, nullptr if this is the last row. We use this patch to determine
 * 				the starting point corner of where the seam on this Patch begins
 */
void Patch::cutLeftBoundary(Patch* top, Patch* bottom)
{
	int overlap = m_dimension / Quilt::OVERLAP_DIVISOR;
	int startY = m_dimension - 1;

	int previousCol = -1; // We keep track as we move in the Y direction where (in X) the previous column was cut at
	// since we can only go within -1 -> +1 from that for the next cut

	if (bottom != nullptr)
	{
		startY = m_dimension - overlap + bottom->getCornerCut()[1] - 1;
		previousCol = bottom->getCornerCut()[0];
	}

	int endY = -1;

	if (top != nullptr)
	{
		endY = getCornerCut()[1];
	}

	for (int y = startY; y >= endY + 1; y--)
	{
		if (y == m_dimension - 1 && bottom == nullptr) // Haven't determined starting cut yet
		{
			int smallest = m_error->getPixelValueAt(0, y);
			int index = 0;

			for (int i = 1; i < overlap; i++) // So we have to iterate the entire row to find the smallest error
			{
				int error = m_error->getPixelValueAt(i, y);

				if (error < smallest)
				{
					smallest = error;
					index = i;
				}
			}
			previousCol = index;
			m_boundaries->setPixelValueAt(previousCol, y, 1);
		}
		else // Now we can only iterate from -1 to +1 of previousCol
		{
			int start = max(0, previousCol - 1);
			int end = min(overlap, previousCol + 1);
			int smallest = INT_MAX;
			int index = 0;

			for (int i = start; i < end; i++)
			{
				int error = m_error->getPixelValueAt(i, y);

				if (error < smallest)
				{
					smallest = error;
					index = i;
				}
			}
			previousCol = index;
			m_boundaries->setPixelValueAt(previousCol, y, 1);
		}
	}
}

/**
 * Gets the point of the corner cut of this patch. This is where the quilting seam starts on the L overlap region
 * @return Size 2 vector with the coordinates of the corner cut
 */
vector<int> Patch::getCornerCut()
{
	vector<int> point(2);

	point[0] = m_cornerCutX;
	point[1] = m_cornerCutY;

	return point;
}

/**
 * Calculates the least cost cut for the top left corner of this patch. Should only be called for patches that have a
 * Patch to their left
 */
void Patch::determineCorner()
{
	int smallest = INT_MAX;
	int indexX = 0;
	int indexY = 0;
	int overlap = m_dimension / Quilt::OVERLAP_DIVISOR;

	for (int y = 0; y < overlap; y++)
	{
		for (int x = 0; x < overlap; x++)
		{
			int error = m_error->getPixelValueAt(x, y);

			if (error < smallest)
			{
				smallest = error;
				indexX = x;
				indexY = y;
			}
		}
	}

	m_boundaries->setPixelValueAt(indexX, indexY, 1);
	m_cornerCutX = indexX;
	m_cornerCutY = indexY;
}