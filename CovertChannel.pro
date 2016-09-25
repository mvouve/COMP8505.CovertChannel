#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T14:52:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CovertChannel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientdriver.cpp \
    netutils.cpp \
    serverdriver.cpp

HEADERS  += mainwindow.h \
    clientdriver.h \
    netutils.h \
    serverdriver.h

FORMS    += mainwindow.ui
