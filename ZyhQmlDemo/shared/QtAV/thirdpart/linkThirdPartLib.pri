linux:!android:{

    contains(QT_ARCH, x86_64) {
#        unix: CONFIG += link_pkgconfig
#        unix: PKGCONFIG += openssl
    }
    else {
#        THIRDPART_DIR=$$PWD/arm-2016
        THIRDPART_DIR=$$PWD/arm-2023
        INCLUDEPATH += $$THIRDPART_DIR/include
        LIBS += -L$$THIRDPART_DIR/libs -lz
        LIBS += -L$$THIRDPART_DIR/libs -lasound
#        LIBS += -L$$THIRDPART_DIR/libs -lssl
#        LIBS += -L$$THIRDPART_DIR/libs -lcrypto
# openssl库已经由SoftKey包含
    }
}
