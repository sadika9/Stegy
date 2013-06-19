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
    DecryptWidget/decryptwidget.cpp \
    EncryptWidget/encryptwidget.cpp \
    AboutDialog/aboutdialog.cpp

HEADERS  += stegymain.h \
    DecryptWidget/decryptwidget.h \
    lsbcrypterrormessages.h \
    EncryptWidget/encryptwidget.h \
    AboutDialog/aboutdialog.h

FORMS    += stegymain.ui \
    DecryptWidget/decryptwidget.ui \
    EncryptWidget/encryptwidget.ui \
    AboutDialog/aboutdialog.ui

INCLUDEPATH += ./ \
    DecryptWidget/ \
    EncryptWidget/ \
    AboutDialog/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ImageViewer/release/ -lImageViewer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ImageViewer/debug/ -lImageViewer
else:unix: LIBS += -L$$OUT_PWD/../ImageViewer/ -lImageViewer

INCLUDEPATH += $$PWD/../ImageViewer
DEPENDPATH += $$PWD/../ImageViewer

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ImageViewer/release/ImageViewer.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ImageViewer/debug/ImageViewer.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../ImageViewer/libImageViewer.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LsbCrypt/release/ -lLsbCrypt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LsbCrypt/debug/ -lLsbCrypt
else:unix: LIBS += -L$$OUT_PWD/../LsbCrypt/ -lLsbCrypt

INCLUDEPATH += $$PWD/../LsbCrypt
DEPENDPATH += $$PWD/../LsbCrypt

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LsbCrypt/release/LsbCrypt.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LsbCrypt/debug/LsbCrypt.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../LsbCrypt/libLsbCrypt.a

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

OTHER_FILES += \
    AboutDialog/AboutDialog.pro

