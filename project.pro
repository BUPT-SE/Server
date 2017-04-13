#-------------------------------------------------
#
# Project created by QtCreator 2017-04-03T19:30:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    attribute.cpp \
    clientblock.cpp

HEADERS  += widget.h \
    attribute.h \
    clientblock.h \
    server.h

FORMS    += widget.ui

QT += network;
