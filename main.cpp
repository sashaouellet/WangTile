#include <iostream>
#include "BMPFile.h"
#include "Tile.h"
#include "TileMap.h"

using namespace std;

int main() {
    BMPFile* tile1 = new BMPFile("tile_GBGB.bmp");
    BMPFile* tile2 = new BMPFile("tile_GBGY.bmp");
    BMPFile* tile3 = new BMPFile("tile_GBRB.bmp");
    BMPFile* tile4 = new BMPFile("tile_GBRY.bmp");
    BMPFile* tile5 = new BMPFile("tile_GYGB.bmp");
    BMPFile* tile6 = new BMPFile("tile_GYGY.bmp");
    BMPFile* tile7 = new BMPFile("tile_GYRB.bmp");
    BMPFile* tile8 = new BMPFile("tile_GYRY.bmp");
    BMPFile* tile9 = new BMPFile("tile_RYRY.bmp");
    BMPFile* tile10 = new BMPFile("tile_RBGB.bmp");
    BMPFile* tile11 = new BMPFile("tile_RYRB.bmp");
    BMPFile* tile12 = new BMPFile("tile_RBGY.bmp");
    BMPFile* tile13 = new BMPFile("tile_RBRB.bmp");
    BMPFile* tile14 = new BMPFile("tile_RBRY.bmp");
    BMPFile* tile15 = new BMPFile("tile_RYGB.bmp");
    BMPFile* tile16 = new BMPFile("tile_RYGY.bmp");

    vector<Tile> tileSet;

    tileSet.push_back(Tile(tile1));
    tileSet.push_back(Tile(tile2));
    tileSet.push_back(Tile(tile3));
    tileSet.push_back(Tile(tile4));
    tileSet.push_back(Tile(tile5));
    tileSet.push_back(Tile(tile6));
    tileSet.push_back(Tile(tile7));
    tileSet.push_back(Tile(tile8));
    tileSet.push_back(Tile(tile9));
    tileSet.push_back(Tile(tile10));
    tileSet.push_back(Tile(tile11));
    tileSet.push_back(Tile(tile12));
    tileSet.push_back(Tile(tile13));
    tileSet.push_back(Tile(tile14));
    tileSet.push_back(Tile(tile15));
    tileSet.push_back(Tile(tile16));

    TileMap map(tileSet, 10, 10);

    map.generate();
    map.print();

    BMPFile::writeFile(map.getPixelWidth(), map.getPixelHeight(), map.makeArray(), "output.bmp");

    return 0;
}