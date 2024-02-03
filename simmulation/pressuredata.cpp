#include "pressuredata.h"
#include <QColor>
#include "qdebug.h"


PressureData::PressureData() {}

PressureData::~PressureData()
{

}

void PressureData::calculatePressureRMS(float *pressureData, std::vector<Transducer *>* transducers, PointGridData *pointGridData)
{
    qDebug() << "transducers size:" << transducers->size();
    calculatePressureRMS_CPU(pressureData, transducers, pointGridData);
}

void PressureData::calculatePressureRMS_CPU(float *pressureData, std::vector<Transducer *> *transducers, PointGridData *pointGridData)
{
    int resolutionX = pointGridData->resolutionX();
    int resolutionY = pointGridData->resolutionY();
    int resolutionZ = pointGridData->resolutionZ();
    float gridSize = pointGridData->gridSize();
    QVector3D gridCenter = pointGridData->gridCenter();
    float scaleFactor = pointGridData->getScaleFactor();

    float xRootPos = -(resolutionX/2.0f)*gridSize + gridCenter.x();
    float yRootPos = -(resolutionY/2.0f)*gridSize + gridCenter.y();
    float zRootPos = -(resolutionZ/2.0f)*gridSize + gridCenter.z();

    std::vector<float> data(pointGridData->getTotalGridPoint(),0);
    float min=std::numeric_limits<float>::max();
    float max=std::numeric_limits<float>::min();

    for(int x = 0; x < resolutionX; x++)
    {
        for(int y = 0; y < resolutionY; y++)
        {
            for(int z = 0; z < resolutionZ; z++) {
                int idx = x*resolutionY*resolutionZ + y*resolutionZ + z;
                QVector3D targetPt(
                    (xRootPos + x*gridSize),
                    (yRootPos + y*gridSize),
                    (zRootPos + z*gridSize)
                    );
                float reAcc=0;
                float imAcc=0;
                for (int t = 0; t < transducers->size(); t++) {
                    QVector3D delta = targetPt - transducers->at(t)->pos();
                    // qDebug() << "delta:" << delta;
                    float r = delta.length();
                    float dot = delta.x()*transducers->at(t)->normal().x() + delta.y()*transducers->at(t)->normal().y() + delta.z()*transducers->at(t)->normal().z();
                    float lenSq1 = r;
                    float lenSq2 = transducers->at(t)->normal().length();
                    float theta = acos(dot/(lenSq1 * lenSq2));
                    float phi = transducers->at(t)->getRelativeRMSPhase();
                    float Df=2*_j1(m_waveNumber*transducers->at(t)->diameter()/2*sinf(theta)) /
                               (m_waveNumber*transducers->at(t)->diameter()/2*sinf(theta));
                    float re = m_vpp * transducers->at(t)->P0() * Df / r * cosf(phi+m_waveNumber*r);
                    float im = m_vpp * transducers->at(t)->P0() * Df / r * sinf(phi+m_waveNumber*r);

                    reAcc += re;
                    imAcc += im;
                }
                float mag = sqrtf(reAcc*reAcc + imAcc*imAcc);
                // qDebug() << "idx:" << idx;
                // qDebug() << "mag:" << mag;
                data[idx]=mag;
                if(mag>max)
                    max=mag;
                else if(mag<min)
                    min=mag;
            }
        }
    }

    qDebug() << "min:" << min;
    qDebug() << "max:" << max;
    int idx = 0;
    for(int x=0; x<resolutionX; x++)
    {
        for(int y= 0; y<resolutionY; y++)
        {
            for(int z= 0; z<resolutionZ; z++)
            {
                float percent = (data[idx])/(max);
                // qDebug() << "percent:" << percent;
                pressureData[idx] = percent;
                idx ++;
            }
        }
    }
}

extern "C" double* addVectorsGPU(double* a, double* b, int n);

void PressureData::calculatePressureRMS_GPU(float *pressureData, std::vector<Transducer *> *transducers, PointGridData *pointGridData)
{
    int n = 1<<27;
    double* a;
    double* b;
    auto r2 = addVectorsGPU(a, b, n);
}
