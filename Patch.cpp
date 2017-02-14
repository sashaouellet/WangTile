/**
 * The Patch class defines a square piece of a large input image that will then be "quilted" together along with other
 * patches in order to synthesize a larger overall output texture.
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/13/17
 */

#include "Patch.h"

/**
 * The default constructor for the Patch. Uses the given unsigned char array as its pixel data map.
 * @param data The pixel data for this particular patch. This is derived from a pre-determined square dimension segment
 *             of the original input image
 */
Patch::Patch(unsigned char *data)
{
    m_pixelData = data;
}

/**
 * Gets the pixel data array of this patch
 * @return The pixel data array
 */
unsigned char* Patch::getPixelData()
{
    return m_pixelData;
}
