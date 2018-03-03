#-------------------------------------------------
#
# Project created by QtCreator 2018-02-01T09:33:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_test2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    fibheap.cpp \
    NNode.cpp \
    algorithm_implement.cpp \
    cv_process.cpp \
    action.cpp

HEADERS += \
        mainwindow.h \
    fibheap.h

FORMS += \
        mainwindow.ui

INCLUDEPATH +=/usr/local/include
#LIBS += -L/usr/local/lib/
#LIBS +=-L/usr/local/lib -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_imgcodecs

# The following lines tells Qmake to use pkg-config for opencv
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig

PKGCONFIG += opencv

DISTFILES += \
    fibtest.opt \
    fibtest.dsp \
    fibtest.dsw \
    fibtest.ncb \
    desktop.ini \
    fibtest.001 \
    fibtest.plg \
    readme.asc
