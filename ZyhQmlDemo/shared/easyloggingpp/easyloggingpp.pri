
QT     += core gui
CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS *= -std=c++11

HEADERS += \
    $$PWD/src/easylogging++.h

SOURCES += \
    $$PWD/src/easylogging++.cc

INCLUDEPATH += \
    $$PWD/src

#easylogging macros
DEFINES += ELPP_THREAD_SAFE  ELPP_NO_DEFAULT_LOG_FILE
#DEFINES += ELPP_QT_LOGGING ELPP_UNICODE
unix {
DEFINES += ELPP_FEATURE_CRASH_LOG
}










