#-------------------------------------------------
#
# Project created by QtCreator 2020-10-30T17:08:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OperationSystemDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    util.cpp \
    addjobwidget.cpp \
    ramwidget.cpp \
    settings.cpp \
    initwidget.cpp

HEADERS  += mainwindow.h \
    pcb.h \
    util.h \
    pool.h \
    system.h \
    addjobwidget.h \
    ramwidget.h \
    subarea.h \
    settings.h \
    initwidget.h

FORMS    += mainwindow.ui \
    addjobwidget.ui \
    ramwidget.ui \
    settings.ui \
    initwidget.ui

RESOURCES += \
    resources.qrc
