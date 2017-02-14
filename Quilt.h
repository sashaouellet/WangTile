/**
 * The Quilt class specifies the synthesized "quilted" texture of any number of Patches. Patches are generated from an
 * input image and then randomly selected, overlapped with each other, and an error surface calculated, upon which
 * a least-cost boundary cut on both edges of the patch in order to fit them together "seamlessly"
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/13/17
 */

#ifndef WANGTILE_QUILT_H
#define WANGTILE_QUILT_H

#include "BMPFile.h"
#include "Patch.h"
#include <vector>

using namespace std;

class Quilt
{
private:
    BMPFile *m_source;
    unsigned int m_dimension;
    unsigned int m_patchSize;
    vector<Patch*> m_patches;

    const int OVERLAP_DIVISOR = 6;

    void extractPatches();

public:
    Quilt(BMPFile *, unsigned int, unsigned int);
    vector<vector<Patch*>> generate();
    vector<Patch*> getPatches();
    void addPatch(Patch*, Patch*, vector<Patch*>);
};

#endif //WANGTILE_QUILT_H
