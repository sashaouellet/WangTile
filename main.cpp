#include <iostream>
#include "BMPFile.h"
#include "Tile.h"
#include "TileMap.h"

using namespace std;

int main() {
    BMPFile* tile1 = new BMPFile("tile_GBGC.bmp");
    BMPFile* tile2 = new BMPFile("tile_GCYC.bmp");
    BMPFile* tile3 = new BMPFile("tile_GRGB.bmp");
    BMPFile* tile4 = new BMPFile("tile_KBGR.bmp");
    BMPFile* tile5 = new BMPFile("tile_YCKC.bmp");
    BMPFile* tile6 = new BMPFile("tile_YRGC.bmp");
    BMPFile* tile7 = new BMPFile("tile_YRGR.bmp");
    BMPFile* tile8 = new BMPFile("tile_YRYC.bmp");

    vector<Tile> tileSet;

    tileSet.push_back(Tile(tile1));
    tileSet.push_back(Tile(tile2));
    tileSet.push_back(Tile(tile3));
    tileSet.push_back(Tile(tile4));
    tileSet.push_back(Tile(tile5));
    tileSet.push_back(Tile(tile6));
    tileSet.push_back(Tile(tile7));
    tileSet.push_back(Tile(tile8));

    TileMap map(tileSet, 5, 2);

    map.generate();
    map.print();



    return 0;
}

