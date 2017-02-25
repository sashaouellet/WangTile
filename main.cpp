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
    test3();

    return 0;
}

// Quilt patch extraction test
void test3()
{
    BMPFile file("test.bmp");
    Quilt quilt(file, 256, 64);

    vector<vector<Patch*>> patches = quilt.generate();
    vector<vector<Tile>> tiles;
    unsigned int patchSize = quilt.getPatchSize();
    tiles.resize(patches.size());

	for (int i = 0 ; i < patches.size() ; i++)
	{
		for (int j = 0 ; j < patches[i].size() ; j++)
		{
            vector<char> c = {'a', 'b', 'c', 'd'};
            Tile t(*new BMPFile(patches[i][j]->getPixelData(), patchSize, patchSize), c);
			tiles[i].push_back(t);
		}
	}

    TileMap map(tiles, 4, 4);
    unsigned char *data = map.makeArray();

    BMPFile::writeFile(256, 256, data, "testPatches.bmp");
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
    BMPFile tile1("tile_GBGB.bmp");
    BMPFile tile2("tile_GBGY.bmp");
    BMPFile tile3("tile_GBRB.bmp");
    BMPFile tile4("tile_GBRY.bmp");
    BMPFile tile5("tile_GYGB.bmp");
    BMPFile tile6("tile_GYGY.bmp");
    BMPFile tile7("tile_GYRB.bmp");
    BMPFile tile8("tile_GYRY.bmp");
    BMPFile tile9("tile_RYRY.bmp");
    BMPFile tile10("tile_RBGB.bmp");
    BMPFile tile11("tile_RYRB.bmp");
    BMPFile tile12("tile_RBGY.bmp");
    BMPFile tile13("tile_RBRB.bmp");
    BMPFile tile14("tile_RBRY.bmp");
    BMPFile tile15("tile_RYGB.bmp");
    BMPFile tile16("tile_RYGY.bmp");

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