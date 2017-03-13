/**
 * The Quilt class specifies the synthesized "quilted" texture of any number of Patches. Patches are generated from an
 * input image and then randomly selected, overlapped with each other, and an error surface calculated, upon which
 * a least-cost boundary cut on both edges of the patch in order to fit them together "seamlessly"
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/13/17
 */

#ifndef WANGTILE_QUILT_H
#define WANGTILE_QUILT_H

#include "BMPFile.h"
#include "Patch.h"
#include "Tile.h"
#include <vector>
#include <random>

using namespace std;

class Quilt
{
private:
    BMPFile& m_source;
    int m_dimension;
    int m_patchesPerSide;
    int m_patchSize;
    vector<Patch*> m_patchSet;
	vector<vector<Patch*>> m_patches;
	RGBPlane* m_output;
    default_random_engine m_generator;

    void extractPatches();
	void layoutPatches(vector<Patch*>);
    void setOutputPixel(Patch*, int, int, int, int);

public:
    const static int OVERLAP_DIVISOR = 6;
    constexpr static double BEST_FIT_MARGIN = 1.1;

    Quilt(BMPFile&, int, int);
	Quilt(BMPFile&, int, vector<Patch*>);
    void generate();
    Patch* getPatch(Patch*, Patch*);
	Patch* getRandom(vector<Patch*>&, bool);
    int getDimension();
	RGBPlane* makeSeamsAndQuilt();
    vector<vector<Patch*>> getPatches();
    RGBPlane* getOutput();
    Tile* getTile();

	Patch* getPatchFromSourceAt(int, int, int, int, char);
    static Patch* getPatchFromSourceAt(BMPFile&, int, int, int, int, int, char);

    virtual ~Quilt();
};

#endif //WANGTILE_QUILT_H
