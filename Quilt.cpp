/**
 * The Quilt class specifies the synthesized "quilted" texture of any number of Patches. Patches are generated from an
 * input image and then randomly selected, overlapped with each other, and an error surface calculated, upon which
 * a least-cost boundary cut on both edges of the patch in order to fit them together "seamlessly"
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/13/17
 */

#include <limits.h>
#include <chrono>
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
    m_generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

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
            m_patches.push_back(new Patch(m_source->getPixelRegion(colLower, rowLower, colLower + m_patchSize - 1, rowLower + m_patchSize - 1), m_patchSize));
        }
    }
}

vector<vector<Patch*>> Quilt::generate()
{
    vector<vector<Patch*>> patches;
	unsigned int patchesPerSide = m_dimension / m_patchSize;

	for (int i = 0; i < patchesPerSide; i++)
	{
		vector<Patch*> row;

		for (int j = 0; j < patchesPerSide; j++)
		{
			Patch *left = j != 0 ? row[j - 1] : nullptr;
			Patch *above = i != 0 ? patches[i - 1][j] : nullptr;

			row.push_back(getPatch(left, above));
		}

        patches.push_back(row);
	}

	return patches;
}

/**
 * Returns the next patch. This determines a random patch from a subset of the entire
 * patch set that satisfies the minimum overlap error. Then makes a least-cost cut along the boundary of the top/left
 * edges of the patch.
 *
 * @param left The patch to the left of the patch to be placed, nullptr if the patch to be placed is the first in the row
 * @param above The patch above the patch to be placed, nullptr if this is the first row of patches
 */
Patch* Quilt::getPatch(Patch *left, Patch *above)
{
	// First patch in whole quilt, just pick a random one
	if (left == nullptr && above == nullptr)
	{
		return getRandom(m_patches, false);
	}

	vector<Patch*> patches;
	vector<Patch*>::iterator it;
	unsigned int bestError = UINT_MAX;

	// Loop through once to calculate overlap region errors
    for (it = m_patches.begin() ; it < m_patches.end() ; it++)
    {
        Patch *patch = new Patch((*it)->getPixelData(), (*it)->getDimension());
        unsigned int error = patch->getOverlapScore(left, above);
        bestError = error < bestError ? error : bestError;

        patches.push_back(patch);
    }

	// Loop again to trim patches that fall outside of acceptable margin of error
    for (it = patches.begin(); it != patches.end();)
    {
        // Not within X% of least error
        if ((*it)->getTotalError() > bestError * Quilt::BEST_FIT_MARGIN)
        {
            it = patches.erase(it);
        }
        else
        {
            it++;
        }
    }

    return getRandom(patches, false);
}

/**
 * Gets a random patch from the given patch list
 *
 * @param del Boolean determining if the non-selected patch pointers should be deleted
 * @return The randomly selected patch
 */
Patch* Quilt::getRandom(vector<Patch*> &patches, bool del)
{
	uniform_int_distribution<int> dist(0, patches.size() - 1);
    int ind = dist(m_generator);

    if (del)
    {
        for (int i = 0; i < patches.size(); i++)
        {
            if (i != ind)
            {
                delete patches[i];
            }
        }
    }

	return patches[ind];
}

/**
 * Gets the patch set for this quilt
 * @return The patch set
 */
vector<Patch*> Quilt::getPatches()
{
    return m_patches;
}
