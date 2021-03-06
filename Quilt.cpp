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
 * @param patchesPerSide The number of patches to make along each side of the sqaure quilt
 * @param patchSize The side length of each patch that will be extracted from the source bitmap. Larger patch size
 *                  increases efficiency of quilting, but the output will be repetitive. Too small a patch size may
 *                  result in a loss of key details from the source image.
 */
Quilt::Quilt(BMPFile& source, int patchesPerSide, int patchSize)
 : m_source(source) {
    if (source.getWidth() % patchSize != 0)
    {
        throw invalid_argument("Patch size must be whole divisor of input source image");
    }

    int overlap = patchSize / Quilt::OVERLAP_DIVISOR;

    m_dimension = (patchesPerSide * patchSize) - ((patchesPerSide - 1) * overlap);
    m_patchesPerSide = patchesPerSide;
    m_patchSize = patchSize;
    m_generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    m_output = new RGBPlane(m_dimension, m_dimension);

    extractPatches();
}

/**
 * Creates a quilt from a predetermined arrangement of patches
 *
 * @param source Where this quilt's patches were extracted from
 * @param patchesPerSide The number of patches per side in order to determine the 2D grid layout
 * @param patches The patch list that this quilt is composed from
 */
Quilt::Quilt(BMPFile& source, int patchesPerSide, vector<Patch*> patches)
 : m_source(source) {
	Patch* p = patches[0];
	int patchSize = p->getDimension();
	int overlap = patchSize / Quilt::OVERLAP_DIVISOR;

	m_dimension = (patchesPerSide * patchSize) - ((patchesPerSide - 1) * overlap);
	m_patchesPerSide = patchesPerSide;
	m_patchSize = patchSize;
	m_generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	m_output = new RGBPlane(m_dimension, m_dimension);

	layoutPatches(patches);
}

Quilt::~Quilt()
{
//    vector<Patch*>::iterator it;
//    for (it = m_patchSet.begin() ; it < m_patchSet.end(); it++)
//    {
//        delete *it;
//    }

//    vector<vector<Patch*>>::iterator it2;
//
//    for (it2 = m_patches.begin() ; it2 < m_patches.end() ; it2++)
//    {
//        vector<Patch*>::iterator it3;
//
//        for (it3 = (*it2).begin() ; it3 < (*it2).end() ; it3++)
//        {
//            delete *it3;
//        }
//    }

//    delete m_output;
}

/**
 * Extracts patches from the source image to create a patch set that can then be called upon to quilt the final output
 * image together
 */
void Quilt::extractPatches()
{
    int patchesPerSide = m_source.getWidth() / m_patchSize;

    for (int i = 0 ; i < patchesPerSide ; i++)
    {
        int rowLower = i * m_patchSize;
        for (int j = 0 ; j < patchesPerSide ; j++)
        {
            int colLower = j * m_patchSize;
            m_patchSet.push_back(Quilt::getPatchFromSourceAt(colLower, rowLower, colLower + m_patchSize - 1, rowLower + m_patchSize - 1, 0));
        }
    }
}

Patch* Quilt::getPatchFromSourceAt(int x1, int y1, int x2, int y2, char code)
{
	return new Patch(*m_source.getPlane()->getRegion(x1, y1, x2, y2, true), m_patchSize, code);
}

Patch* Quilt::getPatchFromSourceAt(BMPFile& source, int patchSize, int x1, int y1, int x2, int y2, char code)
{
    return new Patch(*source.getPlane()->getRegion(x1, y1, x2, y2, false), patchSize, code);
}

/**
 * Lays out the given patches according to the already defined number of patches per side in this Quilt
 *
 * @param patches The patches to layout
 */
void Quilt::layoutPatches(vector<Patch*> patches)
{
	int index = 0;

	for (int i = 0; i < m_patchesPerSide; i++)
	{
		vector<Patch*> row;

		for (int j = 0; j < m_patchesPerSide; j++)
		{
            Patch* p = patches[index++];
            Patch* left = j != 0 ? patches[(i * m_patchesPerSide) + (j - 1)] : nullptr;
            Patch* top = i != 0 ? patches[((i - 1) * m_patchesPerSide) + j] : nullptr;

            p->getOverlapScore(left, top);
			row.push_back(p);
		}

		m_patches.push_back(row);
	}
}

void Quilt::generate()
{
	for (int i = 0; i < m_patchesPerSide; i++)
	{
		vector<Patch*> row;

		for (int j = 0; j < m_patchesPerSide; j++)
		{
			Patch* left = j != 0 ? row[j - 1] : nullptr;
			Patch* above = i != 0 ? m_patches[i - 1][j] : nullptr;

			row.push_back(getPatch(left, above));
		}

        m_patches.push_back(row);
	}
}

RGBPlane* Quilt::makeSeamsAndQuilt()
{
	for (int i = 0; i < m_patchesPerSide; i++)
	{
        cout << "ROW: " << i << endl;
		for (int j = 0; j < m_patchesPerSide; j++)
		{
            cout << "\tCOL: " << j << endl;
			Patch* left = j != 0 ? m_patches[i][j - 1] : nullptr;
			Patch* top = i != 0 ? m_patches[i - 1][j] : nullptr;

			m_patches[i][j]->calculateLeastCostBoundaries(left, top);

            for (int y = 0; y < m_patchSize; y++)
            {
                for (int x = 0; x < m_patchSize; x++)
                {
                    setOutputPixel(m_patches[i][j], j, i, x, y);
                }
            }
		}
	}

    return m_output;
}

/**
 * Sets the value of the output plane's pixel given the specified patch
 * @param patch The patch to extract the specific pixel from
 * @param patchPosX The x position of the patch in the space of THIS QUILT. This is the patch's position
 * @param patchPosY The y position of the patch in the space of THIS QUILT. This is the patch's position
 * @param x The x position of the PIXEL in the patch
 * @param y The y position of the PIXEL in the patch
 */
void Quilt::setOutputPixel(Patch* patch, int patchPosX, int patchPosY, int x, int y)
{
    int overlap = m_patchSize / Quilt::OVERLAP_DIVISOR;
    int quiltX = (patchPosX * (m_patchSize - overlap)) + x;
    int quiltY = (patchPosY * (m_patchSize - overlap)) + y;

    int* pixel = patch->getPixelAt(x, y);
    unsigned char mask = (unsigned char) patch->getBoundaries()->getPixelValueAt(x, y);

    if (mask)
    {
        m_output->setPixelValueAt(quiltX, quiltY, (unsigned char) pixel[0], (unsigned char) pixel[1], (unsigned char) pixel[2], false);
    }
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
        Patch* p = getRandom(m_patchSet, false);
		return new Patch(*p);
	}

	vector<Patch*> patches;
	vector<Patch*>::iterator it;
	int bestError = INT_MAX;

	// Loop through once to calculate overlap region errors
    for (it = m_patchSet.begin() ; it < m_patchSet.end() ; it++)
    {
        Patch *patch = new Patch(**it);
        int error = patch->getOverlapScore(left, above);
        bestError = error < bestError ? error : bestError;

        patches.push_back(patch);
    }

	// Loop again to trim patches that fall outside of acceptable margin of error
    for (it = patches.begin(); it != patches.end();)
    {
        // Not within X% of least error
        if ((*it)->getTotalError() > bestError * Quilt::BEST_FIT_MARGIN)
        {
            delete *it;
            it = patches.erase(it);
        }
        else
        {
            it++;
        }
    }

    return getRandom(patches, true);
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
 * Gets the output RGBPlane generated after the seam process has been completed
 *
 * @return The output of the seam process
 */
RGBPlane* Quilt::getOutput()
{
    return m_output;
}

/**
 * Gets the dimension of the quilt (side length)
 *
 * @return The size, in pixels, of of the Quilt edges
 */
int Quilt::getDimension()
{
    return m_dimension;
}

vector<vector<Patch*>> Quilt::getPatches()
{
    return m_patches;
}

/**
 * Gets the tile from the rotated center of this quilt, with the coded edges taken from the codes of each of the patches
 * this Quilt is composed from
 *
 * @return The tile from this Quilt's center
 */
Tile* Quilt::getTile()
{
    RGBPlane* tilePlane = getOutput()->rotate();

    vector<char> codes = {m_patches[0][0]->getCode(), m_patches[0][1]->getCode(), m_patches[1][1]->getCode(), m_patches[1][0]->getCode()};

    int offset = tilePlane->getWidth() / 4 + 6;
    int width = (tilePlane->getWidth() / 2) - 12;

    RGBPlane* region = tilePlane->getRegion(offset, offset, offset + width, offset + width, true);

    BMPFile file = BMPFile(*region);

    return new Tile(file, codes);
}
