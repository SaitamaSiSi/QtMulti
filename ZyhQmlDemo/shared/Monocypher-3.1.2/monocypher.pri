
QMAKE_CXXFLAGS *= -std=c++11
QMAKE_CFLAGS *= -std=c99

SOURCES += \
    $$PWD/monocypher.c

HEADERS += \
    $$PWD/monocypher.h
    
INCLUDEPATH += $$PWD/


