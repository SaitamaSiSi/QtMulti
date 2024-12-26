
CONFIG += c++11
QMAKE_CXXFLAGS *= -std=c++11

# 使用静态库
# DEFINES += FACEDETECTION_STATIC_DEFINE
# 添加并行库(因使用静态库需要指定链接)
# unix: LIBS += -lgomp

contains(QT_ARCH, x86_64) {
    # contains 'Desktop Linux'
    message("libfacedetection release version : Desktop Linux")

    unix:!macx: LIBS += -L$$PWD/output/x86/lib/ -lfacedetection

    INCLUDEPATH += $$PWD/output/x86/include
    DEPENDPATH += $$PWD/output/x86/include

    #unix:!macx: PRE_TARGETDEPS += $$PWD/output/x86/lib/libfacedetection.a

 } else {

    # contains 'Embedded Linux'
    message("libfacedetection release version : Embedded Linux")

    unix:!macx: LIBS += -L$$PWD/output/arm/lib/ -lfacedetection

    INCLUDEPATH += $$PWD/output/arm/include
    DEPENDPATH += $$PWD/output/arm/include

    #unix:!macx: PRE_TARGETDEPS += $$PWD/output/arm/lib/libfacedetection.a
}





