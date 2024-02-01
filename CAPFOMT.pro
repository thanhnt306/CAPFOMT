QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    display\axis.cpp \
    main.cpp \
    display\mainwindow.cpp \
    display\pointgrid.cpp \
    simmulation\pointgriddata.cpp \
    simmulation\pressuredata.cpp \
    display\simwidget.cpp \
    simmulation\transducer.cpp \
    business\transducerarrangement.cpp

HEADERS += \
    business\Setting.h \
    display\axis.h \
    display\mainwindow.h \
    display\pointgrid.h \
    simmulation\pointgriddata.h \
    simmulation\pressuredata.h \
    display\simwidget.h \
    simmulation\transducer.h \
    simmulation\transducerarrangement.h

FORMS += \
    display\mainwindow.ui \
    display\simwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders/axis.frag \
    shaders/axis.vert \
    shaders/pointcloud.frag \
    shaders/pointcloud.vert
