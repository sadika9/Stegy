TEMPLATE = subdirs

SUBDIRS += \
    ImageViewer \
    LsbSteg \
    #StegyMain/DecryptWidget \
    #StegyMain/EncryptWidget \
    StegyMain

CONFIG += ordered

QMAKE_CXXFLAGS += -std=c++11
