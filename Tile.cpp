/**
 * The Tile class represents the Wang Tile construct, holding information about its associated bitmap "texture," and the
 * color codes of the N, E, S, W sides of the tile face.
 */

#include "Tile.h"

/**
 * Default constructor for the Tile object, given the bitmap file to make this tile from
 */
Tile::Tile(BMPFile* file)
{
    m_image = file;
}

char Tile::getCodeAtSide(int)
{
    return 0;
}

bool Tile::hasCodeAtSide(char, int)
{
    return false;
}
