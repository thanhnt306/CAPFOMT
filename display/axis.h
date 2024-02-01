#ifndef AXIS_H
#define AXIS_H

#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class Axis
{
public:
    Axis();
    void initializeGL();
    void paintGL(QMatrix4x4 &camera, QMatrix4x4 &projection);

private:
     QOpenGLShaderProgram *m_program = nullptr;
    int m_projMatrixLoc = 0;
    int m_camMatrixLoc = 0;
    QOpenGLBuffer *m_vbo = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;
};

#endif // AXIS_H
