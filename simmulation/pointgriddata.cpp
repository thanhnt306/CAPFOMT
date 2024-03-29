#include "pointgriddata.h"
#include "qdebug.h"

PointGridData::PointGridData()
{

}

PointGridData::~PointGridData()
{

}

void PointGridData::initializePoints(float *pointsData)
{
    // int positiveXPointNum = m_resolutionX/2u + m_resolutionX%2;
    // int negativeXPointNum = positiveXPointNum - m_resolutionX%2;
    float xRootPos = -(m_resolutionX/2.0f)*m_gridSize + m_gridCenter.x;
    float yRootPos = -(m_resolutionY/2.0f)*m_gridSize + m_gridCenter.y;
    float zRootPos = -(m_resolutionZ/2.0f)*m_gridSize + m_gridCenter.z;

    for(int z = 0; z < m_resolutionZ; z++)
    {
        for(int y = 0; y < m_resolutionY; y++)
        {
            for(int x = 0; x < m_resolutionX; x++) {
                *pointsData++ = (xRootPos + x*m_gridSize)*scaleFactor;
                *pointsData++ = (yRootPos + y*m_gridSize)*scaleFactor;
                *pointsData++ = (zRootPos + z*m_gridSize)*scaleFactor;
            }
        }
    }
}

int PointGridData::getTotalGridPoint()
{
    return m_resolutionX * m_resolutionY * m_resolutionZ;
}

int PointGridData::resolutionX() const
{
    return m_resolutionX;
}

int PointGridData::resolutionY() const
{
    return m_resolutionY;
}

int PointGridData::resolutionZ() const
{
    return m_resolutionZ;
}

float PointGridData::gridSize() const
{
    return m_gridSize;
}

Vec3D PointGridData::gridCenter() const
{
    return m_gridCenter;
}

float PointGridData::getScaleFactor() const
{
    return scaleFactor;
}
