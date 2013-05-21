#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T23:29:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EncryptWidget
TEMPLATE = app


SOURCES += main.cpp\
        encryptwidget.cpp

HEADERS  += encryptwidget.h \
    ../DecryptWidget/lsbcrypterrormessages.h

FORMS    += encryptwidget.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ImageViewer/release/ -lImageViewer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ImageViewer/debug/ -lImageViewer
else:unix: LIBS += -L$$OUT_PWD/../../ImageViewer/ -lImageViewer

INCLUDEPATH += $$PWD/../../ImageViewer
DEPENDPATH += $$PWD/../../ImageViewer

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ImageViewer/release/ImageViewer.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ImageViewer/debug/ImageViewer.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../ImageViewer/libImageViewer.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../LsbCrypt/release/ -lLsbCrypt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../LsbCrypt/debug/ -lLsbCrypt
else:unix: LIBS += -L$$OUT_PWD/../../LsbCrypt/ -lLsbCrypt

INCLUDEPATH += $$PWD/../../LsbCrypt
DEPENDPATH += $$PWD/../../LsbCrypt

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../LsbCrypt/release/LsbCrypt.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../LsbCrypt/debug/LsbCrypt.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../LsbCrypt/libLsbCrypt.a
