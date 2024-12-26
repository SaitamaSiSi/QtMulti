
QT     += core gui
CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS *= -std=c++11

# VNC
#DEFINES += ENABLE_VNC

# 日志模块
DEFINES += ENABLE_ELOGGERWRAPPER

# cpp-httplib
# 当使用MinGW编译时手动指定包含win32
#ifndef _MSC_VER
win32:LIBS += -lws2_32
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

# Use Precompiled headers (PCH)
#CONFIG += precompile_header
#PRECOMPILED_HEADER  = $$PWD/precompileheaders.hpp

INCLUDEPATH += $$PWD/

include($$PWD/cpp-httplib-0.11.3/cpp-httplib.pri)
include($$PWD/nlohmann-json-3.11.2/nlohmann-json.pri)
include($$PWD/Monocypher-3.1.2/monocypher.pri)
include($$PWD/OpenSSL/openssl.pri)


