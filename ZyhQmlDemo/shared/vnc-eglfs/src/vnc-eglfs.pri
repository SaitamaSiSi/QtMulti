#TEMPLATE = lib
#TARGET   = qvnceglfs

#DEFINES += VNC_MAKEDLL

QT += gui gui-private network

CONFIG += hide_symbols
CONFIG += silent
CONFIG += no_private_qt_headers_warning

CONFIG += strict_c++
CONFIG += c++11
CONFIG += warn_on

HEADERS += \
    $$PWD/RfbSocket.h \
    $$PWD/RfbPixelStreamer.h \
    $$PWD/RfbEncoder.h \
    $$PWD/RfbInputEventHandler.h \
    $$PWD/VncServer.h \
    $$PWD/VncClient.h \
    $$PWD/VncNamespace.h \

SOURCES += \
    $$PWD/RfbSocket.cpp \
    $$PWD/RfbPixelStreamer.cpp \
    $$PWD/RfbEncoder.cpp \
    $$PWD/RfbInputEventHandler.cpp \
    $$PWD/VncServer.cpp \
    $$PWD/VncClient.cpp \
    $$PWD/VncNamespace.cpp \

INCLUDEPATH += $$PWD
