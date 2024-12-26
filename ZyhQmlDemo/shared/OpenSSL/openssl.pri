
QMAKE_CXXFLAGS *= -std=c++11
QMAKE_CFLAGS *= -std=c99

SOURCES += \
    $$PWD/rsahelper.cpp \
    $$PWD/base64helper.cpp

HEADERS += \
    $$PWD/rsahelper.h \
    $$PWD/base64helper.h
    
INCLUDEPATH += $$PWD/

#---------------------------------------------------------------------------------------
# For openssl,we must add openssl lib for compile it
#---------------------------------------------------------------------------------------

contains(QT_ARCH, x86_64) {

    # contains 'Desktop Linux'
    message("openssl编译目标平台:x86")
    unix: CONFIG += link_pkgconfig
    unix: PKGCONFIG += openssl

 } else {

    # contains 'Embedded Linux'
    message("openssl编译目标平台:arm")
    #OPENSSLSRC=$$PWD/depends/arm-2016
    OPENSSLSRC=$$PWD/depends/arm-2023
    unix:!macx: LIBS += -L$$OPENSSLSRC/libs -lssl
    unix:!macx: LIBS += -L$$OPENSSLSRC/libs -lcrypto
    INCLUDEPATH += $$OPENSSLSRC/include
    DEPENDPATH += $$OPENSSLSRC/include

}


