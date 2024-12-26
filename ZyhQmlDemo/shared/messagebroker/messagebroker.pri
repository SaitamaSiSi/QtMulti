
QT     += core gui
CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS *= -std=c++11

HEADERS += \
    $$PWD/publisher.h \
    $$PWD/subscriber.h \
    $$PWD/subscriberbroker.h

SOURCES += \
    $$PWD/publisher.cpp \
    $$PWD/subscriber.cpp \
    $$PWD/subscriberbroker.cpp

