#-------------------------------------------------
#
# Project created by QtCreator 2013-05-19T21:05:25
#
#-------------------------------------------------

QT       += core gui

TARGET = LsbSteg
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ../SimpleCrypt/simplecrypt.cpp \
    lsbsteg.cpp

HEADERS += \
    passtokey.h \
    ../SimpleCrypt/simplecrypt.h \
    lsbsteg.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

win32:INCLUDEPATH += F:\\opencv\\include

win32:LIBS += -LF:\\opencv\\lib \
    -lopencv_core244.dll \
    -lopencv_highgui244.dll \
    -lopencv_imgproc244.dll \
    -lopencv_features2d244.dll \
    -lopencv_calib3d244.dll



QMAKE_CXXFLAGS += -std=c++11
