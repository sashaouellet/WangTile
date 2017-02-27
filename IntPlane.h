/**
 * The IntPlane class represents an XY plane of pixels, and the integer value each of these pixels holds.
 *
 * @author Sasha Ouellet - spaouellet@me.com - www.sashaouellet.com
 * @version 1.0 - 02/25/17
 */

#ifndef WANGTILE_INTPLANE_H
#define WANGTILE_INTPLANE_H


class IntPlane
{
private:
    int* m_pixelData;
    int m_width;
    int m_height;

    int getIndexFromPoint(int, int);

public:
    IntPlane(int, int);
    IntPlane(const IntPlane&);
    int getPixelValueAt(int, int);
    void setPixelValueAt(int, int, int);

    virtual ~IntPlane();
};

#endif //WANGTILE_INTPLANE_H