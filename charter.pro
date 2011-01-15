#-------------------------------------------------
#
# Project created by QtCreator 2010-11-07T17:58:32
#
#-------------------------------------------------

QT       += core gui

TARGET = charter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    samplingthread.cpp \
    signaldata.cpp \
    positiondatax.cpp \
    positiondatay.cpp \
    positiondataz.cpp \
	sampledata.cpp \
	distancedataup.cpp \
	distancedataright.cpp \
	distancedataleft.cpp \
    positionplot.cpp \
    distanceplot.cpp \
    sample.cpp

HEADERS  += mainwindow.h \
    samplingthread.h \
    sample.h \
    signaldata.h \
    positiondatax.h \
    positiondatay.h \
    positiondataz.h \
	sampledata.h \
	distancedataup.h \
	distancedataright.h \
	distancedataleft.h \
    positionplot.h \
    distanceplot.h

FORMS    += mainwindow.ui

LIBS += -lqwt -lQtSerialPort -L/usr/local/qserialport/lib/

INCLUDEPATH += /usr/local/qserialport/include/QtSerialPort
