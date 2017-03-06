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
    Patch* m_left;
    Patch* m_right;
    Patch* m_top;
    Patch* m_bottom;
    int m_dimension;
    int m_totalError;
	int m_cornerCutX;
	int m_cornerCutY;

public:
    Patch(const RGBPlane&, int);
    Patch(const Patch&);
    RGBPlane* getRGBPlane() const;
    IntPlane* getErrorPlane() const;
	IntPlane* getBoundaries() const;
    Patch* getLeft();
    Patch* getRight();
    Patch* getTop();
    Patch* getBottom();
    void setNeighbours(Patch*, Patch*, Patch*, Patch*);
    int getOverlapScore(Patch*, Patch*);
    int getDimension();
    int* getPixelAt(int, int);
    int getTotalError();
    void calculateLeastCostBoundaries(bool);
	void cutTopBoundary();
	void cutLeftBoundary();
	vector<int> getCornerCut();
	void determineCorner();
    void interpolateMask();
    vector<vector<int>> findBestHorizontalPath(int, int, int, int);

    virtual ~Patch();
};

#endif //WANGTILE_PATCH_H
