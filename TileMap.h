/**
 * Represents the spread of all the tiles that will be outputted to the final bitmap file
 *
 * Tiles are stored in a vector of vectors, to represent the grid layout of the Tiles.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/05/17
 * @version 1.1 - 02/19/17 - Allowing construction via pre-developed vector array of Tiles
 */

#ifndef WANGTILE_TILEMAP_H
#define WANGTILE_TILEMAP_H

#include "Tile.h"
#include <random>

class TileMap
{
private:
    vector<vector<Tile>> m_tiles;
    vector<Tile> m_tileSet;
    int m_width;
    int m_height;
    default_random_engine m_generator;
    uniform_int_distribution<int> m_distribution;

public:
    TileMap(vector<Tile>&, unsigned int, unsigned int);
	TileMap(vector<vector<Tile>>&, unsigned int, unsigned int);
    void generate();
    Tile getRandom();
    void print();
    unsigned char* makeArray();
    void placeTile(Tile&, int, int, unsigned char*);
    int getPixelWidth();
    int getPixelHeight();
	Tile getTileAt(int, int);
	//TODO: addTile to original tile set method
};

#endif //WANGTILE_TILEMAP_H
