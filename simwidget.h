#ifndef SIMWIDGET_H
#define SIMWIDGET_H

#include "axis.h"
#include "pointgrid.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#define deg2rad(x) (x/180.0*3.14159)

namespace Ui {
class SimWidget;
}

class SimWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit SimWidget(QWidget *parent = nullptr);
    ~SimWidget();

protected:
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void initializeGL() override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    Ui::SimWidget *ui;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_world;

    QVector3D m_eye;
    QVector3D m_target = {0, 0, 0};
    float eyeDistance = 5.0f;

    QPoint m_lastPos;
    float yaw = 90.0f;
    float pitch = 0.0f;

    Axis axis;
    PointGrid pointGrid;
    void setEyeCoordinates();
};

#endif // SIMWIDGET_H
