#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_simWidget = ui->SimWidgetWindow;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PointSizeSlider_valueChanged(int value)
{
    m_simWidget->PointSizeChanged(value);
}


void MainWindow::on_PointMaxValSlider_valueChanged(int value)
{
    m_simWidget->PointMaxValueChanged(value);
}


void MainWindow::on_PointMinValSlider_valueChanged(int value)
{
    m_simWidget->PointMinValueChanged(value);
}

