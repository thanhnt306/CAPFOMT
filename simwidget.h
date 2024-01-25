#ifndef SIMWIDGET_H
#define SIMWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

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

    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLBuffer *m_vbo = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;
    int m_projMatrixLoc = 0;
    int m_camMatrixLoc = 0;
};

#endif // SIMWIDGET_H
