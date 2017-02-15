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

class Patch
{
private:
    unsigned char* m_pixelData;
    unsigned int* m_error;
    unsigned int m_dimension;
    unsigned int m_totalError;

public:
    Patch(unsigned char *, unsigned int);
    unsigned char* getPixelData();
    unsigned int getOverlapScore(Patch*, Patch*);
    unsigned int getDimension();
    unsigned int* getPixelAt(int, int);
    unsigned int getTotalError();

    virtual ~Patch();
};

#endif //WANGTILE_PATCH_H
