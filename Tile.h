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
    static const int NORTH = 0;
    static const int EAST = 1;
    static const int SOUTH = 2;
    static const int WEST = 3;

    Tile(BMPFile*);
    char getCodeAtSide(int);
    bool hasCodeAtSide(char, int);
    void print();
};


#endif //WANGTILE_TILE_H
