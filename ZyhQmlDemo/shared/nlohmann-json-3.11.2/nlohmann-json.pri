QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS += -std=c99

HEADERS += \
    $$PWD/single_include/nlohmann/json.hpp \
    $$PWD/single_include/nlohmann/json_fwd.hpp 

INCLUDEPATH += $$PWD/single_include
