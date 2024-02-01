#include "transducer.h"

Transducer::Transducer(QVector3D pos, QVector3D normal)
{
    m_pos = pos;
    m_normal = normal;
}

QVector3D Transducer::pos() const
{
    return m_pos;
}

QVector3D Transducer::normal() const
{
    return m_normal;
}

float Transducer::getRelativeRMSPhase() const
{
    return relativeRMSPhase;
}

float Transducer::diameter() const
{
    return m_diameter;
}

float Transducer::P0() const
{
    return m_P0;
}
