#ifndef POINTGRIDDATA_H
#define POINTGRIDDATA_H

#include "business/setting.h"
#include "business/Vec3D.h"

class PointGridData
{
public:
    explicit PointGridData();
    ~PointGridData();
    void initializePoints(float *pointsData);
    int getTotalGridPoint();

    int resolutionX() const;

    int resolutionY() const;

    int resolutionZ() const;

    float gridSize() const;

    Vec3D gridCenter() const;

    float getScaleFactor() const;

private:

    int m_resolutionX = 200; // number of point
    int m_resolutionY = 198;
    int m_resolutionZ = 1;

    float scaleFactor = 10.0f;

    float m_gridSize = 0.001f; //    m
    Vec3D m_gridCenter = Vec3D(0.0f,0.1f,0.0f);
};

#endif // POINTGRIDDATA_H
