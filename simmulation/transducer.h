#ifndef TRANSDUCER_H
#define TRANSDUCER_H

#include "business/vec3d.h"

class Transducer
{
public:

#ifdef __NVCC__
    __device__
#endif
    Transducer(){}

#ifdef __NVCC__
    __device__
#endif
    Transducer(Vec3D pos = Vec3D(0,0,0), Vec3D normal= Vec3D(0,1,0))
    {
        m_pos = pos;
        m_normal = normal;
    };

#ifdef __NVCC__
    __device__
#endif
        Vec3D pos() const
        {
            return m_pos;
        };

#ifdef __NVCC__
    __device__
#endif
        Vec3D normal() const
        {
            return m_normal;
        };

#ifdef __NVCC__
    __device__
#endif
        float getRelativeRMSPhase() const
        {
            return relativeRMSPhase;
        };

#ifdef __NVCC__
    __device__
#endif
        float diameter() const
        {
            return m_diameter;
        };

#ifdef __NVCC__
    __device__
#endif
        float P0() const
        {
            return m_P0;
        };

private:
    Vec3D m_pos;
    Vec3D m_normal;
    float m_diameter = 0.007f; //    m
    float m_P0 = 1.0;
    float realtimePhase= 0.0f;
    float relativeRMSPhase = 0.0f;
};

#endif // TRANSDUCER_H
