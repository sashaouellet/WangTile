/**
 * The Tile class represents the Wang Tile construct, holding information about its associated bitmap "texture," and the
 * color codes of the N, E, S, W sides of the tile face.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/05/17
 * @version 1.1 - 02/18/17 - New constructor to specify in advance the side codes rather than deriving from filename
 */

#include "Tile.h"
#include "util.h"

/**
 * Default constructor for the Tile object, given the bitmap file to make this tile from
 *
 * @param The bitmap file representing this Tile's pixel content
 */
Tile::Tile(BMPFile& file)
: m_image(file) {
    m_sideCodes = util::parseFileNameForSideCodes(string(file.getFileName()), '_');
}

/**
 * Constructs the Tile with the specified bitmap file and the side codes vector representing the "codes" for each of the
 * sides so that the Tile may be tiled properly
 *
 * @param file The bitmap file
 * @param codes The vector representing each side's code
 */
Tile::Tile(BMPFile& file, vector<char>& codes)
: m_image(file) {
	m_sideCodes = codes;
}

Tile::Tile(const Tile& tile)
: m_image(tile.m_image) {
	m_sideCodes = tile.m_sideCodes;
}

/**
 * Given the index, returns the code of the specified side. The sides are stored as follows:
 *
 * N: 0
 * E: 1
 * S: 2
 * W: 3
 *
 * (Clockwise formation)
 *
 * @param index The index to retrieve (the side)
 * @return The associated code for that side
 * @throws invalid_argument if the index is not within 0-3
 */
char Tile::getCodeAtSide(int index)
{
    if (index < 0 || index > 3)
    {
        throw invalid_argument("Index out of bounds. Must be between 0-3");
    }
    return m_sideCodes[index];
}

/**
 * Determines if this Tile has the particular code at the given side. Useful for the actual tiling of the plane,
 * to quickly check for Tiles that could fit at a particular location
 *
 * @return true if it has this code, false if not
 */
bool Tile::hasCodeAtSide(char code, int index)
{
    return code == getCodeAtSide(index);
}

/**
 * Prints the Tile information: namely its code sequence
 */
void Tile::print()
{
    cout << m_sideCodes[0] << m_sideCodes[1] << m_sideCodes[2] << m_sideCodes[3];
}

/**
 * Get the image this Tile is associated with
 * @return The tile image
 */
BMPFile& Tile::getImage()
{
    return m_image;
}

/**
 * Gets the size of this tile's side length
 * @return The tile side length
 */
int Tile::getDimension()
{
    return m_image.getWidth();
}

/**
 * Determines if the given Tile is the same as this one
 * @param other The other tile to compare to
 * @return True if the other tile has matching side codes, false if not or other is nullptr
 */
bool Tile::isSame(Tile* other)
{
    if (other == nullptr)
    {
        return false;
    }

    for (int i = 0; i < 4; i++)
    {
        if (other->getCodeAtSide(i) != getCodeAtSide(i))
        {
            return false;
        }
    }

    return true;
}
