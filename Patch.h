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
    int m_dimension;
    int m_totalError;
	int m_cornerCutX;
	int m_cornerCutY;
	char m_code;

public:
    Patch(const RGBPlane&, int, char);
    Patch(const Patch&);
    RGBPlane* getRGBPlane() const;
    IntPlane* getErrorPlane() const;
	IntPlane* getBoundaries() const;
    int getOverlapScore(Patch*, Patch*);
    int getDimension();
    int* getPixelAt(int, int);
    int getTotalError();
    void calculateLeastCostBoundaries(Patch*, Patch*);
    vector<int*> getVerticalCut();
    vector<int*> getHorizontalCut();
	char getCode();

	virtual ~Patch();

	static const char CODE_R = 'r';
	static const char CODE_B = 'b';
	static const char CODE_G = 'g';
	static const char CODE_Y = 'y';

private:
	void cutTopBoundary(Patch*);
	void cutLeftBoundary(Patch*);
	vector<int> findCorner();
};

#endif //WANGTILE_PATCH_H
