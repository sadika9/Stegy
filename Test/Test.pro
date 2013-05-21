QT += core gui widgets

SOURCES += \
    main.cpp

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

RESOURCES +=

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv


win32:INCLUDEPATH += F:\\opencv\\include

win32:LIBS += -LF:\\opencv\\lib \
    -lopencv_core244.dll \
    -lopencv_highgui244.dll \
    -lopencv_imgproc244.dll \
    -lopencv_features2d244.dll \
    -lopencv_calib3d244.dll
