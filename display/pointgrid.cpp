#include "pointgrid.h"

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

PointGrid::PointGrid()
{
    m_pointData = new PointGridData();
    m_pressureData = new PressureData();
}

void PointGrid::initializeGL(std::vector<Transducer *>* transducers)
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
    m_pointOptionsLoc = m_program->uniformLocation("pointOptions");

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

    delete m_vboPressureRMS;
    m_vboPressureRMS = new QOpenGLBuffer;
    m_vboPressureRMS->create();
    m_vboPressureRMS->bind();
    m_vboPressureRMS->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vboPressureRMS->allocate(totalpoint * sizeof(GLfloat));

    float *pressureData = (float*)m_vboPressureRMS->map(QOpenGLBuffer::WriteOnly);
    m_pressureData->calculatePressureRMS(pressureData,transducers,m_pointData);
    m_vboPressureRMS->unmap();

    qDebug() << "pointsize:" << m_vboPoints->size();
    qDebug() << "pressure:" << m_vboPressureRMS->size();

    delete m_vao;
    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    m_vao->bind();
    m_vboPoints->bind();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    m_vboPressureRMS->bind();
    f->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), nullptr);
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
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
    m_program->setUniformValue(m_pointOptionsLoc, QVector3D(m_ptSize, m_ptMin, m_ptMax));
    // qDebug() << "options:" << QVector3D(m_ptSize, m_ptMin, m_ptMax);

    f->glDrawArrays(GL_POINTS, 0, m_pointData->getTotalGridPoint());
    m_vao->release();
    m_program->release();
}

void PointGrid::PointSizeChanged(int value)
{
    m_ptSize = 5*value/50.0f;
}

void PointGrid::PointMaxValueChanged(int value)
{
    m_ptMax = value/100.0f;
}

void PointGrid::PointMinValueChanged(int value)
{
    m_ptMin = value/100.0f;
}
