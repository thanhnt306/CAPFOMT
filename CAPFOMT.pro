QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    axis.cpp \
    main.cpp \
    mainwindow.cpp \
    pointgrid.cpp \
    pointgriddata.cpp \
    simwidget.cpp

HEADERS += \
    axis.h \
    mainwindow.h \
    pointgrid.h \
    pointgriddata.h \
    simwidget.h

FORMS += \
    mainwindow.ui \
    simwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders/axis.frag \
    shaders/axis.vert \
    shaders/pointcloud.frag \
    shaders/pointcloud.vert
