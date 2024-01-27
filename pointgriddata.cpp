#include "pointgriddata.h"
#include "qdebug.h"

PointGridData::PointGridData(QObject *parent)
    :QObject(parent)
{

}

PointGridData::~PointGridData()
{

}

void PointGridData::initializePoints(float *pointsData)
{
    // int positiveXPointNum = m_resolutionX/2u + m_resolutionX%2;
    // int negativeXPointNum = positiveXPointNum - m_resolutionX%2;
    float xRootPos = -(m_resolutionX/2.0f)*m_gridSize + m_gridCenter.x();
    float yRootPos = -(m_resolutionY/2.0f)*m_gridSize + m_gridCenter.y();
    float zRootPos = -(m_resolutionZ/2.0f)*m_gridSize + m_gridCenter.z();

    for(int x = 0; x < m_resolutionX; x++)
    {
        for(int y = 0; y < m_resolutionY; y++)
        {
            for(int z = 0; z < m_resolutionZ; z++) {
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
