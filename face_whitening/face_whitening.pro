#-------------------------------------------------
#
# Project created by QtCreator 2015-11-12T20:02:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_whitening
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RC_FILE = icon.rc

INCLUDEPATH+=C:\opencv\Mybuild\install\include

LIBS+=C:\opencv\Mybuild\install\x86\vc11\lib\opencv_*.lib


RESOURCES += \
    res.qrc
