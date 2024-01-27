#include "pointgrid.h"

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

PointGrid::PointGrid()
{
    m_pointData = new PointGridData();
}

void PointGrid::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    delete m_program;
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../CAPFOMT/shaders/pointcloud.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../CAPFOMT/shaders/pointcloud.frag");
    m_program->link();
    m_program->bind();

    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_camMatrixLoc = m_program->uniformLocation("camMatrix");

    delete m_vboPoints;
    m_vboPoints = new QOpenGLBuffer;
    m_vboPoints->create();
    m_vboPoints->bind();
    m_vboPoints->setUsagePattern(QOpenGLBuffer::StaticDraw);
    int totalpoint = m_pointData->getTotalGridPoint();
    m_vboPoints->allocate(totalpoint * sizeof(GLfloat)*3);

    float *pointsData = (float*)m_vboPoints->map(QOpenGLBuffer::WriteOnly);
    m_pointData->initializePoints(pointsData);
    m_vboPoints->unmap();

    delete m_vao;
    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    m_vao->bind();
    m_vboPoints->bind();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    f->glEnableVertexAttribArray(0);
    f->glEnable(GL_DEPTH_TEST);
    m_vao->release();
}

void PointGrid::paintGL(QMatrix4x4 &camera, QMatrix4x4 &projection)
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    m_program->bind();
    m_vao->bind();
    m_program->setUniformValue(m_projMatrixLoc, projection);
    m_program->setUniformValue(m_camMatrixLoc, camera);

    f->glDrawArrays(GL_POINTS, 0, m_pointData->getTotalGridPoint());
    m_vao->release();
    m_program->release();
}
