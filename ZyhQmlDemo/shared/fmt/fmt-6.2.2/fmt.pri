
QT     += core gui
CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS *= -std=c++11

INCLUDEPATH += $$PWD/include

HEADERS += \
    $$PWD/include/fmt/posix.h \
    $$PWD/include/fmt/locale.h \
    $$PWD/include/fmt/ostream.h \
    $$PWD/include/fmt/os.h \
    $$PWD/include/fmt/ranges.h \
    $$PWD/include/fmt/compile.h \
    $$PWD/include/fmt/color.h \
    $$PWD/include/fmt/printf.h \
    $$PWD/include/fmt/chrono.h \
    $$PWD/include/fmt/format-inl.h \
    $$PWD/include/fmt/core.h \
    $$PWD/include/fmt/format.h

SOURCES += \
    $$PWD/src/format.cc \
    $$PWD/src/os.cc











