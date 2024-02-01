#ifndef TRANSDUCER_H
#define TRANSDUCER_H

#include <QObject>
#include <QVector3D>

class Transducer
{
public:
    Transducer(QVector3D pos = QVector3D(0,0,0), QVector3D normal= QVector3D(0,1,0));

    QVector3D pos() const;

    QVector3D normal() const;

    float getRelativeRMSPhase() const;

    float diameter() const;

    float P0() const;

private:
    QVector3D m_pos;
    QVector3D m_normal;
    float m_diameter = 0.007; //    m
    float m_P0 = 1.0;
    float realtimePhase= 0.0f;
    float relativeRMSPhase = 0.0f;
};

#endif // TRANSDUCER_H
