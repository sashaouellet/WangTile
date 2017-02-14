/**
 * The Patch class defines a square piece of a large input image that will then be "quilted" together along with other
 * patches in order to synthesize a larger overall output texture.
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/13/17
 */

#ifndef WANGTILE_PATCH_H
#define WANGTILE_PATCH_H

class Patch
{
private:
    unsigned char* m_pixelData;

public:
    Patch(unsigned char *);
    unsigned char* getPixelData();
};

#endif //WANGTILE_PATCH_H
