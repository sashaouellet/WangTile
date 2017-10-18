/**
 * The RGBPlane class represents an XY plane of RGB values for a pixel plane.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/25/17
 */

#ifndef WANGTILE_RGBPLANE_H
#define WANGTILE_RGBPLANE_H

#include <vector>

using namespace std;

class RGBPlane
{
private:
    unsigned char* m_pixelData;
    int m_width;
    int m_height;

    int getIndexFromPoint(int, int);

public:
    RGBPlane(int, int);
    RGBPlane(const RGBPlane&);
    vector<unsigned char> getPixelValueAt(int, int, bool);
    unsigned char getValueAt(int);
    void setPixelValueAt(int, int, unsigned char, unsigned char, unsigned char, bool);
    RGBPlane* getRegion(int, int, int, int, bool);
    void flipRBValues();
    void setDimensions(int, int);
    int getWidth() const;
    int getHeight() const;
    unsigned char* getRawData();
    RGBPlane* rotate();

    virtual ~RGBPlane();
};

#endif //WANGTILE_RGBPLANE_H