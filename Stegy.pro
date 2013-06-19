TEMPLATE = subdirs

SUBDIRS += \
    ImageViewer \
    LsbCrypt \
    #StegyMain/DecryptWidget \
    #StegyMain/EncryptWidget \
    StegyMain

CONFIG += ordered

QMAKE_CXXFLAGS += -std=c++11
