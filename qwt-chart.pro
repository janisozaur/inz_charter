#-------------------------------------------------
#
# Project created by QtCreator 2010-11-07T17:58:32
#
#-------------------------------------------------

QT       += core gui

TARGET = qwt-chart
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plot.cpp \
    samplingthread.cpp

HEADERS  += mainwindow.h \
    plot.h \
    samplingthread.h

FORMS    += mainwindow.ui

LIBS += -lqwt
