#-------------------------------------------------
#
# Project created by QtCreator 2013-05-19T20:57:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = lib
CONFIG += staticlib

SOURCES += imageviewer.cpp \
    sgraphicsview.cpp

HEADERS += imageviewer.h \
    sgraphicsview.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    imageviewer.ui
