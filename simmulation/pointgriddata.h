#ifndef POINTGRIDDATA_H
#define POINTGRIDDATA_H

#include <QObject>
#include <QVector3D>
#include "business/setting.h"

class PointGridData : QObject
{
    Q_OBJECT
public:
    explicit PointGridData(QObject *parent = nullptr);
    ~PointGridData();
    void initializePoints(float *pointsData);
    int getTotalGridPoint();

    int resolutionX() const;

    int resolutionY() const;

    int resolutionZ() const;

    float gridSize() const;

    QVector3D gridCenter() const;

    float getScaleFactor() const;

private:

    int m_resolutionX = 200; // number of point
    int m_resolutionY = 200;
    int m_resolutionZ = 1;

    float scaleFactor = 10.0f;

    float m_gridSize = 0.001; //    m
    QVector3D m_gridCenter = QVector3D(0.0f,0.1f,0.0f);
};

#endif // POINTGRIDDATA_H
