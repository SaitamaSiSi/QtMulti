
QT     += core gui
CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH += $$PWD/

HEADERS += \
    $$PWD/eloggerwrapper.h

SOURCES += \
    $$PWD/eloggerwrapper.cpp


# depend on easyloggingpp and fmt

DEFINES += ENABLE_ELOGGERWRAPPER








