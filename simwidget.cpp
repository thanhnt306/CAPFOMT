#include "simwidget.h"
#include "ui_simwidget.h"
#include <QOpenGLExtraFunctions>
#include <QMouseEvent>

SimWidget::SimWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::SimWidget)
{
    ui->setupUi(this);
    setEyeCoordinates();
}

SimWidget::~SimWidget()
{
    delete ui;
}

void SimWidget::resizeGL(int width, int height)
{
    m_proj.setToIdentity();
    m_proj.perspective(20.0f, GLfloat(width) / height, 0.01f, 100.0f);
    update();
}

void SimWidget::paintGL()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClearColor(0.2f, 0.2f, 0.2f, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 camera;
    camera.lookAt(m_eye, m_target, QVector3D(0, 1, 0));

    axis.paintGL(camera, m_proj);
    pointGrid.paintGL(camera, m_proj);
}

void SimWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_PROGRAM_POINT_SIZE);

    axis.initializeGL();
    pointGrid.initializeGL();
}

void SimWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void SimWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        const float sensitivity = 100.1f;
        float dx = (event->position().x() - m_lastPos.x()) / width() * sensitivity;
        float dy = -(m_lastPos.y() - event->position().y() ) / height() * sensitivity;

        yaw += dx;
        pitch += dy;

        if(pitch > 89.0f)
            pitch = 89.0f;
        else if(pitch < -89.0f)
            pitch = -89.0f;


        setEyeCoordinates();

        update();
    }

    m_lastPos = event->pos();
}

void SimWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;

    eyeDistance -= numSteps * 0.1;

    if(eyeDistance > 10.0)
        eyeDistance = 10.0;
    else if(eyeDistance<1.0)
        eyeDistance = 1.0;

    setEyeCoordinates();

    update();
}

void SimWidget::setEyeCoordinates()
{
    m_eye.setX( cos(deg2rad(yaw)) * cos(deg2rad(pitch)) * eyeDistance);
    m_eye.setY( sin(deg2rad(pitch)) * eyeDistance);
    m_eye.setZ(sin(deg2rad(yaw)) * cos(deg2rad(pitch)) * eyeDistance);
}
