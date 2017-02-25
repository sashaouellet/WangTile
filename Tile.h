/**
 * The Tile class represents the Wang Tile construct, holding information about its associated bitmap "texture," and the
 * color codes of the N, E, S, W sides of the tile face.
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/05/17
 * @version 1.1 - 02/18/17 - New constructor to specify in advance the side codes rather than deriving from filename
 */

#ifndef WANGTILE_TILE_H
#define WANGTILE_TILE_H

#include <vector>
#include "BMPFile.h"

using namespace std;

class Tile
{
private:
    BMPFile m_image;
    vector<char> m_sideCodes;

public:
    static const int NORTH = 0;
    static const int EAST = 1;
    static const int SOUTH = 2;
    static const int WEST = 3;

    Tile(BMPFile&);
	Tile(BMPFile&, vector<char>&);
	Tile(const Tile&);
    char getCodeAtSide(int);
    bool hasCodeAtSide(char, int);
    void print();
    BMPFile& getImage();
};


#endif //WANGTILE_TILE_H
