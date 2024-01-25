#include "simwidget.h"
#include "ui_simwidget.h"

SimWidget::SimWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::SimWidget)
{
    ui->setupUi(this);
}

SimWidget::~SimWidget()
{
    delete ui;
}

void SimWidget::resizeGL(int width, int height)
{

}

void SimWidget::paintGL()
{

}

void SimWidget::initializeGL()
{

}

void SimWidget::mousePressEvent(QMouseEvent *event)
{

}

void SimWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void SimWidget::wheelEvent(QWheelEvent *event)
{

}
