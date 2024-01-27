#ifndef POINTGRID_H
#define POINTGRID_H

#include "pointgriddata.h"

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class PointGrid
{
public:
    PointGrid();

    void initializeGL();
    void paintGL(QMatrix4x4 &camera, QMatrix4x4 &projection);

private:
    PointGridData *m_pointData;
    QOpenGLShaderProgram *m_program = nullptr;
    int m_projMatrixLoc = 0;
    int m_camMatrixLoc = 0;

    QOpenGLBuffer *m_vboPoints = nullptr;
    QOpenGLBuffer *m_vboPressureValue  = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;
};

#endif // POINTGRID_H
