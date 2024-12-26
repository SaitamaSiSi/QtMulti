# QtAV库编译查看同级目录下压缩包
#
#
#
# QtAV库已经安装到Qt5.9.9 SDK中
#
# "avwidgets" module is required only for qwidget apps. QML apps only need "av" module
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += av avwidgets
} else {
  CONFIG += av avwidgets
}

# ffmpeg
include($$PWD/ffmpeg/linkFFmpeg.pri)

# openssl
include($$PWD/thirdpart/linkThirdPartLib.pri)
