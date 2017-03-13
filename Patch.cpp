/**
 * The Patch class defines a square piece of a large input image that will then be "quilted" together along with other
 * patches in order to synthesize a larger overall output texture.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/13/17
 */

#include <limits.h>
#include "Patch.h"
#include "Quilt.h"

/**
 * The default constructor for the Patch. Uses the given unsigned char array as its pixel data map.
 *
 * @param data The pixel data for this particular patch. This is derived from a pre-determined square dimension segment
 *             of the original input image
 * @param dimension The length of the Patch edges
 */
Patch::Patch(const RGBPlane& plane, int dimension, char code)
{
    m_pixelData = new RGBPlane(plane);
    m_dimension = dimension;
    m_error = new IntPlane(dimension, dimension);
	m_boundaries = new IntPlane(dimension, dimension);
    m_totalError = 0;
	m_cornerCutX = 0;
	m_cornerCutY = 0;
	m_code = code;
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
	m_code = patch.m_code;
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
 * the pixel is untouched. 1 signifies that the cut travels through that pixel.
 *
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
int Patch::getOverlapScore(Patch* left, Patch* top)
{
    int overlap = m_dimension / Quilt::OVERLAP_DIVISOR;

    m_error->fill(0);

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
int Patch::getTotalError()
{
    return m_totalError;
}

/**
 * Gets the side length of the patch
 * @return The side length (pixels) of the patch
 */
int Patch::getDimension()
{
    return m_dimension;
}

/**
 * Calculates the cut that needs to be made through this patch's pixels in order to produce the smallest margin of error
 * when quilting it in relation to the provided left and top patches
 *
 * @param left The patch to the left of this one, nullptr if it is the leftmost one in its row
 * @param top The patch above this one, nullptr if it is the first row
 */
void Patch::calculateLeastCostBoundaries(Patch* left, Patch* top)
{
	m_boundaries->fill(0);

	cutTopBoundary(top);
	cutLeftBoundary(left);

	vector<int> corner = findCorner();

	for (int i = 0; i < m_dimension; i++)
	{
		for (int j = 0; j < m_dimension; j++)
		{
            int val = m_boundaries->getPixelValueAt(j, i);

			if (i < corner[1] && j < corner[0])
			{
				m_boundaries->setPixelValueAt(j, i, 3);
			}
			else if (val == 0)
			{
				m_boundaries->setPixelValueAt(j, i, 1);
			}
            else if (val == 2)
            {
                m_boundaries->setPixelValueAt(j, i, 1);
            }
		}
	}

    for (int i = 0; i < m_dimension; i++)
    {
        for (int j = 0; j < m_dimension; j++)
        {
            int val = m_boundaries->getPixelValueAt(j, i);

            if (val > 1)
            {
                m_boundaries->setPixelValueAt(j, i, 0);
            }
        }
    }
}

/**
 * Makes the cut along the top overlap region, given the Patch above this one
 *
 * @param top The patch above this one, nullptr if it is the first row. In that case, no top boundary should be drawn
 * 		      since there is no overlap. The cut is then just the entire top row of pixels.
 */
void Patch::cutTopBoundary(Patch* top)
{
    if (top == nullptr) // No patch above, therefore there is no overlap
    {
        for (int x = 0; x < m_dimension; x++)
        {
            getBoundaries()->setPixelValueAt(x, 0, m_boundaries->getPixelValueAt(x, 0) + 1);
        }
        return;
    }

    vector<int*> paths = getHorizontalCut();
    int* bestPath = paths[0];

    for (int i = 1; i < paths.size(); i++)
    {
        if (paths[i][m_dimension] <= bestPath[m_dimension])
        {
            bestPath = paths[i];
        }
    }

    for (int i = 0; i < m_dimension; i++)
    {
        int row = bestPath[i];
        m_boundaries->setPixelValueAt(i, row, m_boundaries->getPixelValueAt(i, row) + 1);

        if (row != 0)
        {
            for (int j = row - 1; j >= 0; j--)
            {
                m_boundaries->setPixelValueAt(i, j, 3);
            }
        }
    }
}

/**
 * Makes the cut along the left overlap region, given the Patch to the left of this one
 *
 * @param left The patch to the left of this one, nullptr if this is the leftmost one. In that case, no actual cut
 *             will be made since there is no overlap region. The cut is therefore only the leftmost column of pixels
 */
void Patch::cutLeftBoundary(Patch* left)
{
    if (left == nullptr) // No overlap, only make left column the "cut"
    {
        for (int y = 0; y < m_dimension; y++)
        {
            getBoundaries()->setPixelValueAt(0, y, m_boundaries->getPixelValueAt(0, y) + 1);
        }
        return;
    }

    vector<int*> paths = getVerticalCut();
    int* bestPath = paths[0];

    for (int i = 1; i < paths.size(); i++)
    {
        if (paths[i][m_dimension] <= bestPath[m_dimension])
        {
            bestPath = paths[i];
        }
    }

    for (int i = 0; i < m_dimension; i++)
    {
        int col = bestPath[i];
        m_boundaries->setPixelValueAt(col, i, m_boundaries->getPixelValueAt(col, i) + 1);

        if (col != 0)
        {
            for (int j = col - 1; j >= 0; j--)
            {
                m_boundaries->setPixelValueAt(j, i, 3);
            }
        }
    }
}

/**
 * Finds the corner of the boundary cuts
 *
 * @return A size 2 vector containing the x, y coords of the corner of the boundary cuts
 */
vector<int> Patch::findCorner()
{
	vector<int> point(2);
	for (int i = 0; i < m_dimension; i++)
	{
		for (int j = 0; j < m_dimension; j++)
		{
			if (m_boundaries->getPixelValueAt(j, i) == 2)
			{
				point[0] = j;
				point[1] = i;
			}
		}
	}

	return point;
}

/**
 * Gets the code this patch represents
 * @return The code of the patch
 */
char Patch::getCode()
{
    return m_code;
}

/**
 * Function to determine the least cost cut along the vertical overlap surface.
 *
 * @return Array structured such that the last index holds the running cumulative cost of the path, and the rest holds
 *               the indices of each point we have used for this path
 */
vector<int*> Patch::getVerticalCut()
{
    vector<vector<int*>> paths;
    int overlap = m_dimension / Quilt::OVERLAP_DIVISOR;

    paths.resize(m_dimension);

    for (int i = m_dimension - 1; i >= 0; i--)
    {
        vector<int*> row;
        for (int j = 0; j < overlap; j++) {
            if (i == m_dimension - 1)
            {
                int* pathData = new int[m_dimension + 1];

                pathData[m_dimension] = m_error->getPixelValueAt(j, i);
                pathData[i] = j;

                row.push_back(pathData);
            }
            else
            {
                int start = max(0, j - 1);
                int end = min(overlap - 1, j + 1);
                int* best = paths[i + 1][start];

                for (int x = start + 1; x <= end; x++)
                {
                    int* path = paths[i + 1][x];

                    if (path[m_dimension] <= best[m_dimension])
                    {
                        best = path;
                    }
                }

                int* newPath = new int[m_dimension + 1];

                copy(best, best + m_dimension + 1, newPath);

                newPath[i] = j;
                newPath[m_dimension] = newPath[m_dimension] + m_error->getPixelValueAt(j, i);

                row.push_back(newPath);
            }
        }
        paths[i] = row;
    }

    return paths[0];
}

/**
 * Function to determine the least cost cut along the horizontal overlap surface.
 *
 * @return Array structured such that the last index holds the running cumulative cost of the path, and the rest holds
 *               the indices of each point we have used for this path
 */
vector<int*> Patch::getHorizontalCut()
{
    vector<vector<int*>> paths;
    int overlap = m_dimension / Quilt::OVERLAP_DIVISOR;

    paths.resize(m_dimension);

    for (int i = m_dimension - 1; i >= 0; i--) // i here is the column
    {
        vector<int*> row;
        for (int j = 0; j < overlap; j++) {
            if (i == m_dimension - 1)
            {
                int* pathData = new int[m_dimension + 1];

                pathData[m_dimension] = m_error->getPixelValueAt(i, j);
                pathData[i] = j;

                row.push_back(pathData);
            }
            else
            {
                int start = max(0, j - 1);
                int end = min(overlap - 1, j + 1);
                int* best = paths[i + 1][start];

                for (int x = start + 1; x <= end; x++)
                {
                    int* path = paths[i + 1][x];

                    if (path[m_dimension] <= best[m_dimension])
                    {
                        best = path;
                    }
                }

                int* newPath = new int[m_dimension + 1];

                copy(best, best + m_dimension + 1, newPath);

                newPath[i] = j;
                newPath[m_dimension] = newPath[m_dimension] + m_error->getPixelValueAt(i, j);

                row.push_back(newPath);
            }
        }
        paths[i] = row;
    }

    return paths[0];
}
