#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T08:11:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_recognition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH+=C:\opencv\Mybuild\install\include
INCLUDEPATH+=C:\opencv\Mybuild\install\include\opencv
INCLUDEPATH+=C:\opencv\Mybuild\install\include\opencv2

LIBS+=C:\opencv\Mybuild\install\x86\vc11\lib\opencv_*.lib
