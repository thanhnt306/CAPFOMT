#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "simwidget.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PointSizeSlider_valueChanged(int value);

    void on_PointMaxValSlider_valueChanged(int value);

    void on_PointMinValSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    SimWidget* m_simWidget;
};
#endif // MAINWINDOW_H
