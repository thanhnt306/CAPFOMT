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
    // calculatePressureRMS_CPU(pressureData, transducers, pointGridData);
    calculatePressureRMS_GPU(pressureData, transducers, pointGridData);
}

void PressureData::calculatePressureRMS_CPU(float *pressureData, std::vector<Transducer *> *transducers, PointGridData *pointGridData)
{
    int resolutionX = pointGridData->resolutionX();
    int resolutionY = pointGridData->resolutionY();
    int resolutionZ = pointGridData->resolutionZ();
    float gridSize = pointGridData->gridSize();
    Vec3D gridCenter = pointGridData->gridCenter();

    float xRootPos = -(resolutionX/2.0f)*gridSize + gridCenter.x;
    float yRootPos = -(resolutionY/2.0f)*gridSize + gridCenter.y;
    float zRootPos = -(resolutionZ/2.0f)*gridSize + gridCenter.z;

    std::vector<float> data(pointGridData->getTotalGridPoint(),0);
    float min=std::numeric_limits<float>::max();
    float max=std::numeric_limits<float>::min();

    for(int z = 0; z < resolutionZ; z++)
    {
        for(int y = 0; y < resolutionY; y++)
        {
            for(int x = 0; x < resolutionX; x++) {
                int idx = z*resolutionY*resolutionX + y*resolutionX + x;
                Vec3D targetPt(
                    (xRootPos + x*gridSize),
                    (yRootPos + y*gridSize),
                    (zRootPos + z*gridSize)
                    );
                float reAcc=0;
                float imAcc=0;
                for (int t = 0; t < transducers->size(); t++) {
                    Vec3D delta = targetPt - transducers->at(t)->pos();
                    float r = delta.length();
                    float dot = delta.x*transducers->at(t)->normal().x + delta.y*transducers->at(t)->normal().y + delta.z*transducers->at(t)->normal().z;
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
                data[idx]=mag;
                if(mag>max)
                    max=mag;
                else if(mag<min)
                    min=mag;
            }
        }
    }

    int idx = 0;
    for(int z= 0; z<resolutionZ; z++)
    {
        for(int y= 0; y<resolutionY; y++)
        {
            for(int x=0; x<resolutionX; x++)
            {
                float percent = (data[idx])/(max);
                pressureData[idx] = percent;
                idx ++;
            }
        }
    }
}

extern "C" void magCalc(float* magData, std::vector<Transducer *> *transducers, PointGridData *pointGridData, PressureParameter pressureParameter);

void PressureData::calculatePressureRMS_GPU(float *pressureData, std::vector<Transducer *> *transducers, PointGridData *pointGridData)
{
    float* magData = (float*)malloc(pointGridData->resolutionX()
                                  * pointGridData->resolutionY()
                                  * pointGridData->resolutionZ() * sizeof(float));

    PressureParameter prePara = PressureParameter(m_waveNumber, m_vpp);
    magCalc(magData, transducers, pointGridData, prePara);

    int idx = 0;
    float max=std::numeric_limits<float>::min();

    for(int z = 0; z < pointGridData->resolutionZ(); z++ )
    {
        for(int y = 0; y < pointGridData->resolutionY(); y++)
        {
            for(int x = 0; x < pointGridData->resolutionX(); x++)
            {
                if(magData[idx]>max)
                    max=magData[idx];
                idx++;
            }
        }
    }

    idx =0;

    for(int z = 0; z < pointGridData->resolutionZ(); z++)
    {
        for(int y = 0; y < pointGridData->resolutionY(); y++)
        {
            for(int x = 0; x < pointGridData->resolutionX(); x++)
            {
                pressureData[idx] = magData[idx] / max;
                // qDebug() << idx << "---" << magData[idx];
                idx++;
            }
        }
    }
    free(magData);
}
