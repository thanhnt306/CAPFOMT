#ifndef POINTGRIDDATA_H
#define POINTGRIDDATA_H

#include <QObject>
#include <QVector3D>

class PointGridData : QObject
{
    Q_OBJECT
public:
    explicit PointGridData(QObject *parent = nullptr);
    ~PointGridData();
    void initializePoints(float *pointsData);
    int getTotalGridPoint();

private:

    int m_resolutionX = 20; // number of point
    int m_resolutionY = 20;
    int m_resolutionZ = 20;

    float scaleFactor = 0.02;

    float m_gridSize = 1; //mm
    QVector3D m_gridCenter = QVector3D(0.0f,0.0f,0.0f);
};

#endif // POINTGRIDDATA_H
