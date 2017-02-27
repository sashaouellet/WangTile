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
    m_totalError = 0;
}

Patch::Patch(const Patch &patch)
{
    m_pixelData = new RGBPlane(*patch.getRGBPlane());
    m_dimension = patch.m_dimension;
    m_error = new IntPlane(*patch.getErrorPlane());
    m_totalError = 0;
}

Patch::~Patch()
{
    delete m_pixelData;
    delete m_error;
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