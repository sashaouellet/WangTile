/**
 * The Quilt class specifies the synthesized "quilted" texture of any number of Patches. Patches are generated from an
 * input image and then randomly selected, overlapped with each other, and an error surface calculated, upon which
 * a least-cost boundary cut on both edges of the patch in order to fit them together "seamlessly"
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/13/17
 */

#include "Quilt.h"

/**
 * Default constructor for the Quilt. Given the source bitmap image and the patch size, will select all the patches
 * that can be used for the quilting process.
 *
 * @param source The source bitmap image to extract patches from
 * @param dimension The side length of the final outputted, quilted texture
 * @param patchSize The side length of each patch that will be extracted from the source bitmap. Larger patch size
 *                  increases efficiency of quilting, but the output will be repetitive. Too small a patch size may
 *                  result in a loss of key details from the source image.
 */
Quilt::Quilt(BMPFile *source, unsigned int dimension, unsigned int patchSize)
{
    if (source->getWidth() % patchSize != 0)
    {
        throw invalid_argument("Patch size must be whole divisor of input source image");
    }

    m_source = source;
    m_dimension = dimension;
    m_patchSize = patchSize;

    extractPatches();
}

/**
 * Extracts patches from the source image to create a patch set that can then be called upon to quilt the final output
 * image together
 */
void Quilt::extractPatches()
{
    int patchesPerSide = m_source->getWidth() / m_patchSize;

    for (int i = 0 ; i < patchesPerSide ; i++)
    {
        int rowLower = i * m_patchSize;
        for (int j = 0 ; j < patchesPerSide ; j++)
        {
            int colLower = j * m_patchSize;
            m_patches.push_back(new Patch(m_source->getPixelRegion(colLower, rowLower, colLower + m_patchSize - 1, rowLower + m_patchSize - 1)));
        }
    }
}

vector<vector<Patch*>> Quilt::generate()
{
    vector<vector<Patch*>> patches;


}

/**
 * Adds a new random Patch to the given row of the quilt. This determines a random patch from a subset of the entire
 * patch set that satisfies the minimum overlap error. Then makes a least-cost cut along the boundary of the top/left
 * edges of the patch.
 *
 * @param left The patch to the left of the patch to be placed, nullptr if the patch to be placed is the first in the row
 * @param above The patch above the patch to be place, nullptr if this is the first row of patches
 * @param row The patch vector row to add this Patch to
 */
void Quilt::addPatch(Patch *left, Patch *above, vector<Patch*> row)
{

}

/**
 * Gets the patch set for this quilt
 * @return The patch set
 */
vector<Patch*> Quilt::getPatches()
{
    return m_patches;
}
