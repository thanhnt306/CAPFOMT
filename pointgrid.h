#ifndef POINTGRID_H
#define POINTGRID_H

#include "pointgriddata.h"
#include "pressuredata.h"
#include "transducer.h"

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

class PointGrid
{
public:
    PointGrid();

    void initializeGL(std::vector<Transducer *> *transducers);
    void paintGL(QMatrix4x4 &camera, QMatrix4x4 &projection);

    void PointSizeChanged(int value);
    void PointMaxValueChanged(int value);
    void PointMinValueChanged(int value);

private:
    PointGridData *m_pointData;
    PressureData *m_pressureData;
    QOpenGLShaderProgram *m_program = nullptr;
    int m_projMatrixLoc = 0;
    int m_camMatrixLoc = 0;
    int m_pointOptionsLoc = 0;

    QOpenGLBuffer *m_vboPoints = nullptr;
    QOpenGLBuffer *m_vboPressureRMS  = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;

    float m_ptSize=5;
    float m_ptMin=0;
    float m_ptMax=1;
};

#endif // POINTGRID_H
