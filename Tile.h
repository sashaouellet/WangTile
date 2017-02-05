/**
 * The Tile class represents the Wang Tile construct, holding information about its associated bitmap "texture," and the
 * color codes of the N, E, S, W sides of the tile face.
 */

#ifndef WANGTILE_TILE_H
#define WANGTILE_TILE_H

#include <vector>
#include "BMPFile.h"

using namespace std;

class Tile
{
private:
    BMPFile* m_image;
    vector<char> m_sideCodes;

public:
    Tile(BMPFile*);
    char getCodeAtSide(int);
    bool hasCodeAtSide(char, int);
};


#endif //WANGTILE_TILE_H
