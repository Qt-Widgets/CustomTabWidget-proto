#-------------------------------------------------
#
# Project created by QtCreator 2017-02-13T20:25:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qDockwidget_withMenu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    customdockwidget.cpp \
    customtabwidget.cpp \
    drawoverlay.cpp

HEADERS  += mainwindow.h \
    customdockwidget.h \
    customtabwidget.h \
    drawoverlay.h \
    utils.h

FORMS    += mainwindow.ui
