#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25T16:07:30
#
#-------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WormClient1_0
TEMPLATE = app

QT       += network
SOURCES += main.cpp\
        mainwindow.cpp \
    algo.cpp \
    cmdthread.cpp \
    mapthread.cpp \
    values.cpp \
    mapthread.cpp

HEADERS  += mainwindow.h \
    algo.h \
    cmdthread.h \
    consts.h \
    mapthread.h \
    values.h \
    mapthread.h

FORMS    += mainwindow.ui
