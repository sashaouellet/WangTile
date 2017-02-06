/**
 * The Tile class represents the Wang Tile construct, holding information about its associated bitmap "texture," and the
 * color codes of the N, E, S, W sides of the tile face.
 */

#include "Tile.h"
#include "util.h"

/**
 * Default constructor for the Tile object, given the bitmap file to make this tile from
 */
Tile::Tile(BMPFile* file)
{
    m_image = file;
    m_sideCodes = util::parseFileNameForSideCodes(string(file->getFileName()), '_');
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
 * Prints the tile information: namely its code sequence
 */
void Tile::print()
{
    cout << m_sideCodes[0] << m_sideCodes[1] << m_sideCodes[2] << m_sideCodes[3];
}

BMPFile* Tile::getImage()
{
    return m_image;
}
