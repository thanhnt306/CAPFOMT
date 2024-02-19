#ifndef PRESSUREDATA_H
#define PRESSUREDATA_H

#include "pointGridData.h"
#include "transducer.h"

#ifndef M_PI
#define M_PI 3.14159
#endif

struct PressureParameter
{
    float waveNumber;
    float Vpp;
    PressureParameter(float waveNumber, float Vpp):
        waveNumber(waveNumber),
        Vpp(Vpp)
    {

    }
};

class PressureData
{
public:

    PressureData();
    ~PressureData();
    void calculatePressureRMS(float *pressureData,std::vector<Transducer *>* transducers, PointGridData* pointGridData);

private:
    float m_freq = 40000.0f; //    Hz
    float m_soundSpeed = 341.0f; //    m/s
    float m_waveLength = m_soundSpeed/m_freq;
    float m_waveNumber = (float)2*M_PI/m_waveLength;
    float m_vpp = 40.0f; //    V

    void calculatePressureRMS_CPU(float *pressureData,std::vector<Transducer *>* transducers, PointGridData* pointGridData);
    void calculatePressureRMS_GPU(float *pressureData,std::vector<Transducer *>* transducers, PointGridData* pointGridData);
};

#endif // PRESSUREDATA_H
