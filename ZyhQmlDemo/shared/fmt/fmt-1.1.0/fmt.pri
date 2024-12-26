
CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/fmt/format.h
#    $$PWD/fmt/posix.h

SOURCES += \
    $$PWD/fmt/format.cpp
#    $$PWD/fmt/posix.cpp












