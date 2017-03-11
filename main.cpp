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
void test4();
void test5();

int main() {
    test5();

    return 0;
}

void test5()
{
	BMPFile file("/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/flowerpatch.bmp");

	Patch* red =
}

void test4()
{
//	BMPFile file("D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\flowerpatch.bmp");
	BMPFile file("/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/input2.bmp");
	Quilt quilt(file, 10, 64);

	quilt.generate();

	RGBPlane* output = quilt.makeSeamsAndQuilt();

    BMPFile::writeFile(quilt.getDimension(), quilt.getDimension(), output->getRawData(), "/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/imageQuilt_patch64.bmp");
}

// Quilt patch extraction test
void test3()
{
    BMPFile file("test.bmp");
    Quilt quilt(file, 256, 64);

    quilt.generate();
    vector<vector<Patch*>> patches = quilt.getPatches();
    vector<vector<Tile>> tiles;
    tiles.resize(patches.size());

	for (int i = 0 ; i < patches.size() ; i++)
	{
		for (int j = 0 ; j < patches[i].size() ; j++)
		{
            vector<char> c = {'a', 'b', 'c', 'd'};
            Tile t(*new BMPFile(*(patches[i][j]->getRGBPlane())), c);
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
    BMPFile test("flowerpatch.bmp");

    BMPFile::writeFile(41, 41, test.getPlane()->getRegion(0, 0, 40, 40, true).getRawData(), "region.bmp");
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