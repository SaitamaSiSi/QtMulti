
QMAKE_CXXFLAGS *= -std=c++11
QMAKE_CFLAGS *= -std=c99

include($$PWD/../Monocypher-3.1.2/monocypher.pri)
include($$PWD/../OpenSSL/openssl.pri)
SOURCES += \
    $$PWD/keyverify.cpp

HEADERS += \
    $$PWD/keyverify.h \
    $$PWD/keywarnout.h



INCLUDEPATH += $$PWD/../

contains(QT_ARCH, x86_64) {

    # contains 'Desktop Linux'
#    message("crptopp release version : Desktop Linux")
#    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lcryptopp
#    INCLUDEPATH += /usr/include/crypto++
#    INCLUDEPATH += /usr/include/cryptopp
#    DEPENDPATH += /usr/include/crypto++
#    DEPENDPATH += /usr/include/cryptopp


 } else {

    # contains 'Embedded Linux'
#    message("crptopp release version : Embedded Linux")
#    unix:!macx: LIBS += -L$$PWD/../../depends/cryptopp/lib/ -lcryptopp

#    INCLUDEPATH += $$PWD/../../depends/cryptopp/include
#    DEPENDPATH += $$PWD/../../depends/cryptopp/include
#    unix:!macx: LIBS += -L/usr/local/cryptopp_h3/lib/ -lcryptopp

#    INCLUDEPATH += /usr/local/cryptopp_h3/include
#    DEPENDPATH += /usr/local/cryptopp_h3/include
}

RESOURCES += \
    $$PWD/key.qrc


#QMAKE_CXXFLAGS +=  -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable

