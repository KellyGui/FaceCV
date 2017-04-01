#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T11:52:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_detection
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH+=C:\opencv\build\include
INCLUDEPATH+=C:\opencv\build\include\opencv
INCLUDEPATH+=C:\opencv\build\include\opencv2

LIBS+=C:\opencv\build\x86\vc11\lib\opencv_*.lib
