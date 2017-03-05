/**
 * The Patch class defines a square piece of a large input image that will then be "quilted" together along with other
 * patches in order to synthesize a larger overall output texture.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/13/17
 */

#ifndef WANGTILE_PATCH_H
#define WANGTILE_PATCH_H

#include "util.h"
#include "IntPlane.h"
#include "RGBPlane.h"

class Patch
{
private:
    RGBPlane* m_pixelData;
    IntPlane* m_error;
	IntPlane* m_boundaries;
    unsigned int m_dimension;
    unsigned int m_totalError;
	int m_cornerCutX;
	int m_cornerCutY;

public:
    Patch(const RGBPlane&, unsigned int);
    Patch(const Patch&);
    RGBPlane* getRGBPlane() const;
    IntPlane* getErrorPlane() const;
	IntPlane* getBoundaries() const;
    unsigned int getOverlapScore(Patch*, Patch*);
    unsigned int getDimension();
    int* getPixelAt(int, int);
    unsigned int getTotalError();
    void calculateLeastCostBoundaries(Patch*, Patch*, Patch*, Patch*, bool);
	void cutTopBoundary(Patch*, Patch*);
	void cutLeftBoundary(Patch*, Patch*);
	vector<int> getCornerCut();
	void determineCorner();

    virtual ~Patch();
};

#endif //WANGTILE_PATCH_H
