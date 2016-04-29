#-------------------------------------------------
#
# Project created by QtCreator 2016-04-24T19:21:35
#
#-------------------------------------------------
QMAKE_CXXFLAGS += -std=c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qqx5helper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageprocessing.cpp \
    myutil.cpp

HEADERS  += mainwindow.h \
    imageprocessing.h \
    myutil.h

FORMS    += mainwindow.ui
