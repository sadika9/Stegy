#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T21:21:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StegyMain
TEMPLATE = app


SOURCES += main.cpp\
        stegymain.cpp \
    AboutDialog/aboutdialog.cpp \
    DecodeWidget/decodewidget.cpp \
    EncodeWidget/encodewidget.cpp

HEADERS  += stegymain.h \
    lsbstegerrormessages.h \
    AboutDialog/aboutdialog.h \
    DecodeWidget/decodewidget.h \
    EncodeWidget/encodewidget.h

FORMS    += stegymain.ui \
    DecodeWidget/decodewidget.ui \
    EncodeWidget/encodewidget.ui \
    AboutDialog/aboutdialog.ui

INCLUDEPATH += ./ \
    DecodeWidget/ \
    EncodeWidget/ \
    AboutDialog/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ImageViewer/release/ -lImageViewer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ImageViewer/debug/ -lImageViewer
else:unix: LIBS += -L$$OUT_PWD/../ImageViewer/ -lImageViewer

INCLUDEPATH += $$PWD/../ImageViewer
DEPENDPATH += $$PWD/../ImageViewer

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ImageViewer/release/ImageViewer.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ImageViewer/debug/ImageViewer.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../ImageViewer/libImageViewer.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LsbSteg/release/ -lLsbSteg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LsbSteg/debug/ -lLsbSteg
else:unix: LIBS += -L$$OUT_PWD/../LsbSteg/ -lLsbSteg

INCLUDEPATH += $$PWD/../LsbSteg
DEPENDPATH += $$PWD/../LsbSteg

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LsbSteg/release/LsbSteg.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LsbSteg/debug/LsbSteg.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../LsbSteg/libLsbSteg.a

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

win32:INCLUDEPATH += F:\\opencv\\include

win32:LIBS += -LF:\\opencv\\lib \
    -lopencv_core244.dll \
    -lopencv_highgui244.dll \
    -lopencv_imgproc244.dll \
    -lopencv_features2d244.dll \
    -lopencv_calib3d244.dll


RESOURCES += \
    resources.qrc

OTHER_FILES +=

