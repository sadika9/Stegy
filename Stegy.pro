TEMPLATE = subdirs

SUBDIRS += \
    ImageViewer \
    LsbCrypt \
    Test \
    #StegyMain/DecryptWidget \
    #StegyMain/EncryptWidget \
    StegyMain

CONFIG += ordered

QMAKE_CXXFLAGS += -std=c++11
