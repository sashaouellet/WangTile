#include <iostream>
#include <sstream>
#include <limits.h>
#include "BMPFile.h"
#include "Tile.h"
#include "TileMap.h"
#include "Quilt.h"

using namespace std;

void makeQuiltedImage();
void makeWangTiles();

int main() {
<<<<<<< HEAD
	makeWangTiles();
//    makeQuiltedImage();
=======
<<<<<<< HEAD
//    makeWangTiles();
    makeQuiltedImage();
=======
	makeWangTiles();
//    makeQuiltedImage();
>>>>>>> 6a599d761db6c94f29cb834a2274a9e67b1a7369
>>>>>>> dev

    return 0;
}

void makeWangTiles()
{
<<<<<<< HEAD
	BMPFile file("/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/grass.bmp");
=======
<<<<<<< HEAD
	BMPFile file("D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\input2.bmp");
=======
	BMPFile file("/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/grass.bmp");
>>>>>>> 6a599d761db6c94f29cb834a2274a9e67b1a7369
>>>>>>> dev
    int dim = file.getWidth() / 2;

	Patch* red = Quilt::getPatchFromSourceAt(file, dim, 0, 0, dim - 1, dim - 1, Patch::CODE_R);
	Patch* yellow = Quilt::getPatchFromSourceAt(file, dim, dim, 0, (dim * 2) - 1, dim - 1, Patch::CODE_Y);
	Patch* blue = Quilt::getPatchFromSourceAt(file, dim, 0, dim, dim - 1, (dim * 2) - 1, Patch::CODE_B);
	Patch* green = Quilt::getPatchFromSourceAt(file, dim, dim, dim, (dim * 2) - 1, (dim * 2) - 1, Patch::CODE_G);

    vector<Patch*> l1 = {red, yellow, blue, green};
    Quilt q1(file, 2, l1);
    q1.makeSeamsAndQuilt();

    vector<Patch*> l2 = {green, blue, blue, green};
    Quilt q2(file, 2, l2);
    q2.makeSeamsAndQuilt();

    vector<Patch*> l3 = {red, yellow, yellow, red};
    Quilt q3(file, 2, l3);
    q3.makeSeamsAndQuilt();

    vector<Patch*> l4 = {green, blue, yellow, red};
    Quilt q4(file, 2, l4);
    q4.makeSeamsAndQuilt();

    vector<Patch*> l5 = {red, blue, yellow, green};
    Quilt q5(file, 2, l5);
    q5.makeSeamsAndQuilt();

    vector<Patch*> l6 = {green, yellow, yellow, green};
    Quilt q6(file, 2, l6);
    q6.makeSeamsAndQuilt();

    vector<Patch*> l7 = {red, blue, blue, red};
    Quilt q7(file, 2, l7);
    q7.makeSeamsAndQuilt();

    vector<Patch*> l8 = {green, yellow, blue, red};
    Quilt q8(file, 2, l8);
    q8.makeSeamsAndQuilt();

<<<<<<< HEAD
=======
<<<<<<< HEAD
    BMPFile::writeFile(q1.getDimension(), q1.getDimension(), q1.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt1.bmp");
    BMPFile::writeFile(q2.getDimension(), q2.getDimension(), q2.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt2.bmp");
    BMPFile::writeFile(q3.getDimension(), q3.getDimension(), q3.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt3.bmp");
    BMPFile::writeFile(q4.getDimension(), q4.getDimension(), q4.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt4.bmp");
    BMPFile::writeFile(q5.getDimension(), q5.getDimension(), q5.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt5.bmp");
    BMPFile::writeFile(q6.getDimension(), q6.getDimension(), q6.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt6.bmp");
    BMPFile::writeFile(q7.getDimension(), q7.getDimension(), q7.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt7.bmp");
    BMPFile::writeFile(q8.getDimension(), q8.getDimension(), q8.getOutput()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\quilt8.bmp");
=======
>>>>>>> dev
    BMPFile::writeFile(q1.getDimension(), q1.getDimension(), q1.getOutput()->getRawData(), "quilt1.bmp");
    BMPFile::writeFile(q2.getDimension(), q2.getDimension(), q2.getOutput()->getRawData(), "quilt2.bmp");
    BMPFile::writeFile(q3.getDimension(), q3.getDimension(), q3.getOutput()->getRawData(), "quilt3.bmp");
    BMPFile::writeFile(q4.getDimension(), q4.getDimension(), q4.getOutput()->getRawData(), "quilt4.bmp");
    BMPFile::writeFile(q5.getDimension(), q5.getDimension(), q5.getOutput()->getRawData(), "quilt5.bmp");
    BMPFile::writeFile(q6.getDimension(), q6.getDimension(), q6.getOutput()->getRawData(), "quilt6.bmp");
    BMPFile::writeFile(q7.getDimension(), q7.getDimension(), q7.getOutput()->getRawData(), "quilt7.bmp");
    BMPFile::writeFile(q8.getDimension(), q8.getDimension(), q8.getOutput()->getRawData(), "quilt8.bmp");
<<<<<<< HEAD
=======
>>>>>>> 6a599d761db6c94f29cb834a2274a9e67b1a7369
>>>>>>> dev

    Tile* t1 = q1.getTile();
    Tile* t2 = q2.getTile();
    Tile* t3 = q3.getTile();
    Tile* t4 = q4.getTile();
    Tile* t5 = q5.getTile();
    Tile* t6 = q6.getTile();
    Tile* t7 = q7.getTile();
    Tile* t8 = q8.getTile();

<<<<<<< HEAD
=======
<<<<<<< HEAD
    BMPFile::writeFile(t1->getDimension(), t1->getDimension(), t1->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile1.bmp");
    BMPFile::writeFile(t2->getDimension(), t2->getDimension(), t2->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile2.bmp");
    BMPFile::writeFile(t3->getDimension(), t3->getDimension(), t3->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile3.bmp");
    BMPFile::writeFile(t4->getDimension(), t4->getDimension(), t4->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile4.bmp");
    BMPFile::writeFile(t5->getDimension(), t5->getDimension(), t5->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile5.bmp");
    BMPFile::writeFile(t6->getDimension(), t6->getDimension(), t6->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile6.bmp");
    BMPFile::writeFile(t7->getDimension(), t7->getDimension(), t7->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile7.bmp");
    BMPFile::writeFile(t8->getDimension(), t8->getDimension(), t8->getImage().getPlane()->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\tile8.bmp");
=======
>>>>>>> dev
    BMPFile::writeFile(t1->getDimension(), t1->getDimension(), t1->getImage().getPlane()->getRawData(), "tile1.bmp");
    BMPFile::writeFile(t2->getDimension(), t2->getDimension(), t2->getImage().getPlane()->getRawData(), "tile2.bmp");
    BMPFile::writeFile(t3->getDimension(), t3->getDimension(), t3->getImage().getPlane()->getRawData(), "tile3.bmp");
    BMPFile::writeFile(t4->getDimension(), t4->getDimension(), t4->getImage().getPlane()->getRawData(), "tile4.bmp");
    BMPFile::writeFile(t5->getDimension(), t5->getDimension(), t5->getImage().getPlane()->getRawData(), "tile5.bmp");
    BMPFile::writeFile(t6->getDimension(), t6->getDimension(), t6->getImage().getPlane()->getRawData(), "tile6.bmp");
    BMPFile::writeFile(t7->getDimension(), t7->getDimension(), t7->getImage().getPlane()->getRawData(), "tile7.bmp");
    BMPFile::writeFile(t8->getDimension(), t8->getDimension(), t8->getImage().getPlane()->getRawData(), "tile8.bmp");
<<<<<<< HEAD
=======
>>>>>>> 6a599d761db6c94f29cb834a2274a9e67b1a7369
>>>>>>> dev

    vector<Tile> tiles = {*t1, *t2, *t3, *t4, *t5, *t6, *t7, *t8};

    TileMap map(tiles, 5, 5);

    map.generate();
    map.print();

<<<<<<< HEAD
    BMPFile::writeFile(map.getPixelWidth(), map.getPixelHeight(), map.makeArray(), "/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/grassTile.bmp");
=======
<<<<<<< HEAD
    BMPFile::writeFile(map.getPixelWidth(), map.getPixelHeight(), map.makeArray(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\patchTileMapTEST5.bmp");
=======
    BMPFile::writeFile(map.getPixelWidth(), map.getPixelHeight(), map.makeArray(), "/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/grassTile.bmp");
>>>>>>> 6a599d761db6c94f29cb834a2274a9e67b1a7369
>>>>>>> dev
}

void makeQuiltedImage()
{
<<<<<<< HEAD
//	BMPFile file("D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\grass.bmp");
	BMPFile file("/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/grass.bmp");
	Quilt quilt(file, 8, 32);
=======
<<<<<<< HEAD
	BMPFile file("D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\grass.bmp");
//	BMPFile file("/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/input2.bmp");
	Quilt quilt(file, 30, 32);
=======
//	BMPFile file("D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\grass.bmp");
	BMPFile file("/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/grass.bmp");
	Quilt quilt(file, 8, 32);
>>>>>>> 6a599d761db6c94f29cb834a2274a9e67b1a7369
>>>>>>> dev

	quilt.generate();

	RGBPlane* output = quilt.makeSeamsAndQuilt();

<<<<<<< HEAD
    BMPFile::writeFile(quilt.getDimension(), quilt.getDimension(), output->getRawData(), "/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/bricksQuilt.bmp");
=======
<<<<<<< HEAD
    BMPFile::writeFile(quilt.getDimension(), quilt.getDimension(), output->getRawData(), "D:\\Users\\spaouellet\\Documents\\Coding\\VSFX375\\WangTile\\imageQuiltIn2NEW.bmp");
=======
    BMPFile::writeFile(quilt.getDimension(), quilt.getDimension(), output->getRawData(), "/Volumes/Macintosh MD/Users/spaouellet/Documents/code/CLion/WangTile/bricksQuilt.bmp");
>>>>>>> 6a599d761db6c94f29cb834a2274a9e67b1a7369
>>>>>>> dev
}