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
    src/dockingSystem/drawoverlay.cpp \
    src/dockingSystem/splitter.cpp \
    src/dockingSystem/dockwidget.cpp \
    src/dockingSystem/tabwidget.cpp

HEADERS  += mainwindow.h \
    include/dockingSystem/drawoverlay.h \
    utils.h \
    include/dockingSystem/splitter.h \
    include/dockingSystem/dockwidget.h \
    include/dockingSystem/tabwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
