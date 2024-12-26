
CONFIG += c++11
QMAKE_CXXFLAGS *= -std=c++11

unix:!macx: {

INCLUDEPATH += $$PWD/inc

HEADERS += \
    $$PWD/inc/V4l2Access.h \
    $$PWD/inc/V4l2Capture.h \
    $$PWD/inc/V4l2Device.h \
    $$PWD/inc/V4l2MmapDevice.h \
    $$PWD/inc/V4l2Output.h \
    $$PWD/inc/V4l2ReadWriteDevice.h

SOURCES += \
    $$PWD/src/V4l2Access.cpp \
    $$PWD/src/V4l2Capture.cpp \
    $$PWD/src/V4l2Device.cpp \
    $$PWD/src/V4l2MmapDevice.cpp \
    $$PWD/src/V4l2Output.cpp \
    $$PWD/src/V4l2ReadWriteDevice.cpp

}
