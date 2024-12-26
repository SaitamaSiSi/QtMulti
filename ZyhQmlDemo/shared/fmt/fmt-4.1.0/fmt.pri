
CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/fmt/container.h \
    $$PWD/fmt/format.h \
    $$PWD/fmt/ostream.h \
#    $$PWD/fmt/posix.h \
    $$PWD/fmt/printf.h \
    $$PWD/fmt/string.h \
    $$PWD/fmt/time.h

SOURCES += \
    $$PWD/fmt/format.cc \
    $$PWD/fmt/ostream.cc \
#    $$PWD/fmt/posix.cc \
    $$PWD/fmt/printf.cc












