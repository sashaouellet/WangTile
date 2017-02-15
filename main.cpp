#include <iostream>
#include <sstream>
#include "BMPFile.h"
#include "Tile.h"
#include "TileMap.h"
#include "Quilt.h"

using namespace std;

void test1();
void test2();
void test3();

int main() {

//    test3();

	BMPFile f("test.bmp");

	BMPFile::writeFile(256, 256, f.getPixels(), "testOut.bmp");

    return 0;
}

// Quilt patch extraction test
void test3()
{
    Quilt quilt(new BMPFile("flowerpatch.bmp"), 400, 41);

    quilt.generate();
}

// Region extraction and output test
void test2()
{
    BMPFile* test = new BMPFile("flowerpatch.bmp");

    BMPFile::writeFile(41, 41, test->getPixelRegion(0, 0, 40, 40), "region.bmp");
}

// Tiling algorithm test
void test1()
{
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
}