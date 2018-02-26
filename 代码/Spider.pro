#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T20:50:56
#
#-------------------------------------------------

QT       += core gui
QT+=network
QT+=sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spider
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    decisiontree.cpp \
    NeuralNet.cpp

HEADERS  += widget.h \
    decisiontree.h \
    NeuralNet.h

FORMS    += widget.ui
