#ifndef HELPTOOLS_H
#define HELPTOOLS_H

#include <QByteArray>
#include <QColor>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QProcess>
#include <QString>
#include <QTextCodec>
#include <QtEndian>
#include <cstring>
//#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QMap>
#include <QMutex>
#include <QNetworkInterface>
#include <QProcess>
#include <QStringList>
#include <QTextStream>
#include <QThread>
#include <QWaitCondition>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include "precompileheaders.hpp"  //预编译头
#ifdef Q_OS_LINUX
extern "C" {
//系统剩余内存获取
#include <sys/sysinfo.h>
//获取磁盘空间
#include <arpa/inet.h>
#include <errno.h> /*错误号定义*/
#include <fcntl.h> /*文件控制定义*/
#include <malloc.h>
#include <math.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>  /*标准输入输出定义*/
#include <stdlib.h> /*标准函数库定义*/
#include <string.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <termios.h> /*PPSIX 终端控制定义*/
#include <unistd.h>  /*Unix 标准函数定义*/

//#include <sys/mount.h>
}
#endif

#ifdef Q_OS_LINUX
extern "C" {
#include <time.h>
#include <unistd.h>

typedef struct timespec TimeSpecCross;
}
#else
#include <QDateTime>
using TimeSpecCross = QDateTime;
#endif

Q_DECLARE_METATYPE(TimeSpecCross)

#include <QDebug>
#define DBGLINE() \
    do {          \
    } while (0)  //do{qWarning()<<"DBG:"<<__FILE__<<":"<<__LINE__;}while(0)

//辅助工具
class HPT : public QObject
{
public:
    static void setUTF8Code()
    {
        //#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
        //        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        //        QTextCodec::setCodecForLocale(codec);
        //        QTextCodec::setCodecForCStrings(codec);
        //        QTextCodec::setCodecForTr(codec);
        //#endif

        //设置中文编码
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
#if _MSC_VER
        QTextCodec *codec = QTextCodec::codecForName("GBK");
#else
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#endif
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
#else
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
#endif
    }

    static QString GBK2UTF8(QByteArray &data)
    {
        QTextCodec *pCodec = QTextCodec::codecForName("gb2312");
        return pCodec->toUnicode(data.constData(), data.size());
    }

    static QString GBK2UTF8(const char *data)
    {
        QTextCodec *pCodec = QTextCodec::codecForName("gb2312");
        return pCodec->toUnicode(data, (int)std::strlen(data));
    }

    //!
    //! \brief isFileExists 判断文件是否存在
    //! \param filepath
    //! \return true 存在 false 不存在
    //!
    static bool isFileExists(const QString filepath)
    {
        QFileInfo info(filepath);
        return info.isReadable();
    }
    //!
    //! \brief isFileExists 判获取文件大小
    //! \param filepath
    //! \return
    //!
    static qint64 getFileSize(const QString filepath)
    {
        QFileInfo info(filepath);
        return info.size();
    }

    //!
    //! \brief newFile
    //! \param filename
    //! \return
    //! create new file
    static bool newFile(const QString filename)
    {
        bool ret = false;
        if (!isFileExists(filename))
        {
            QFile file(filename);
            ret = file.open(QIODevice::ReadWrite | QIODevice::Text);
            file.close();
        }
        return ret;
    }

    //!
    //! \brief isDirExists
    //! \param dirpath
    //! \return
    //!判断文件夹是否存在
    static bool isDirExists(const QString dirpath)
    {
        QFileInfo info(dirpath);
        return info.isDir();
    }

    //!
    //! \brief mkDir
    //! \param dirpath
    //! \return
    //!创建一级目录
    static bool MKDir(const QString dirpath)
    {
        QDir dir(dirpath);
        return dir.mkdir(dirpath);
    }

    //!
    //! \brief MKDir_p
    //! \param dirpath
    //! \return
    //!创建多级目录
    static bool MKDir_p(const QString dirpath)
    {
        QDir dir(dirpath);
        return dir.mkpath(dirpath);
    }

    //删除目录下所有文件和文件夹
    static bool rmDircontents(const QString &dirpath)
    {
        Q_UNUSED(dirpath)
#ifdef Q_OS_LINUX
        QString cmd = QString("rm -rf ") + dirpath + QString("/*");
        FILE *fp = ::popen(cmd.toStdString().c_str(), "r");
        if (!fp)
        {
            qDebug() << ::strerror(errno);
            return (false);
        }
        ::pclose(fp);
#endif
        return (true);
    }

    //强制删除文件
    static bool rmFileForce(const QString &filepath)
    {
        Q_UNUSED(filepath)
#ifdef Q_OS_LINUX
        QString cmd = QString("rm -rf ") + filepath;
        FILE *fp = ::popen(cmd.toStdString().c_str(), "r");
        if (!fp)
        {
            qDebug() << ::strerror(errno);
            return (false);
        }
        ::pclose(fp);
#else
        QDir dir;
        return dir.remove(filepath);
#endif
        return (true);
    }

    //删除目录
    static bool rmDir(const QString &dirpath)
    {
        Q_UNUSED(dirpath)
#ifdef Q_OS_LINUX

        QString cmd = QString("rm -rf ") + dirpath;
        FILE *fp = ::popen(cmd.toStdString().c_str(), "r");
        if (!fp)
        {
            qDebug() << ::strerror(errno);
            return (false);
        }
        ::pclose(fp);
#else
        QDir dir;
        dir.setPath(dirpath);
        return dir.removeRecursively();
#endif
        return (true);
    }

    //重命名文件
    static bool renamefile(const QString &srcpath, const QString &dstpath)
    {
        if (!QFile::exists(srcpath)) return false;

        if (QFile::exists(dstpath))
        {
            QFile::remove(dstpath);
        }

        return QFile::rename(srcpath, dstpath);
    }

    //复制文件
    static bool copyfile(const QString &srcpath, const QString &dstpath)
    {
        QFile srcfile(srcpath);
        if (srcfile.open(QIODevice::ReadOnly))
        {
            QByteArray srcbytes = srcfile.readAll();

            QFile dstfile(dstpath);
            if (dstfile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                dstfile.write(srcbytes);
                dstfile.flush();
                dstfile.close();
            }
            else
            {
                return false;
            }

            srcfile.close();
        }
        else
        {
            return false;
        }

        return true;
    }

    //删除文件夹下所有内容
    static void rmDircontens(const QString &dstdir)
    {
        QString cmd = QString("rm -rf %1/*").arg(dstdir);
        shellexec(cmd);
        //删除隐藏文件
        QString cmd2 = QString("rm -rf %1/.* 2>/dev/null").arg(dstdir);
        shellexec(cmd2);
    }

    static std::string convertFiletoBase64(const QString &srcpath)
    {
        QFile srcfile(srcpath);
        std::string b64;
        if (srcfile.open(QIODevice::ReadOnly))
        {
            QByteArray srcbytes = srcfile.readAll();
            b64 = srcbytes.toBase64().toStdString();
            srcfile.close();
        }

        return b64;
    }

    static char convertHexChar(char ch)
    {
        if ((ch >= '0') && (ch <= '9'))
        {
            return ch - 0x30;
        }
        else if ((ch >= 'A') && (ch <= 'F'))
        {
            return ch - 'A' + 10;
        }
        else if ((ch >= 'a') && (ch <= 'f'))
        {
            return ch - 'a' + 10;
        }
        else
        {
            return (-1);
        }
    }

    static QByteArray hexStrToByteArray(QString str)
    {
        QByteArray senddata;
        int hexdata, lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len / 2);
        char lstr, hstr;

        for (int i = 0; i < len;)
        {
            hstr = str.at(i).toLatin1();

            if (hstr == ' ')
            {
                i++;
                continue;
            }

            i++;

            if (i >= len)
            {
                break;
            }

            lstr = str.at(i).toLatin1();
            hexdata = convertHexChar(hstr);
            lowhexdata = convertHexChar(lstr);

            if ((hexdata == 16) || (lowhexdata == 16))
            {
                break;
            }
            else
            {
                hexdata = hexdata * 16 + lowhexdata;
            }

            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }

        senddata.resize(hexdatalen);
        return senddata;
    }

    static QString byteArrayToHexStr(QByteArray data)
    {
        QString temp = "";
        QString hex = data.toHex();

        for (int i = 0; i < hex.length(); i = i + 2)
        {
            temp += hex.mid(i, 2) + " ";
        }

        return temp.trimmed().toUpper();
    }

    static QString decimalToStrHex(int decimal)
    {
        QString temp = QString::number(decimal, 16);

        if (temp.length() == 1)
        {
            temp = "0" + temp;
        }

        return temp.toUpper();
    }

    static QString byteArrayToAsciiStr(QByteArray data)
    {
        QString temp;
        int len = data.size();

        for (int i = 0; i < len; i++)
        {
            //0x20为空格,空格以下都是不可见字符
            char b = data.at(i);

            if (0x00 == b)
            {
                temp += QString("\\NUL");
            }
            else if (0x01 == b)
            {
                temp += QString("\\SOH");
            }
            else if (0x02 == b)
            {
                temp += QString("\\STX");
            }
            else if (0x03 == b)
            {
                temp += QString("\\ETX");
            }
            else if (0x04 == b)
            {
                temp += QString("\\EOT");
            }
            else if (0x05 == b)
            {
                temp += QString("\\ENQ");
            }
            else if (0x06 == b)
            {
                temp += QString("\\ACK");
            }
            else if (0x07 == b)
            {
                temp += QString("\\BEL");
            }
            else if (0x08 == b)
            {
                temp += QString("\\BS");
            }
            else if (0x09 == b)
            {
                temp += QString("\\HT");
            }
            else if (0x0A == b)
            {
                temp += QString("\\LF");
            }
            else if (0x0B == b)
            {
                temp += QString("\\VT");
            }
            else if (0x0C == b)
            {
                temp += QString("\\FF");
            }
            else if (0x0D == b)
            {
                temp += QString("\\CR");
            }
            else if (0x0E == b)
            {
                temp += QString("\\SO");
            }
            else if (0x0F == b)
            {
                temp += QString("\\SI");
            }
            else if (0x10 == b)
            {
                temp += QString("\\DLE");
            }
            else if (0x11 == b)
            {
                temp += QString("\\DC1");
            }
            else if (0x12 == b)
            {
                temp += QString("\\DC2");
            }
            else if (0x13 == b)
            {
                temp += QString("\\DC3");
            }
            else if (0x14 == b)
            {
                temp += QString("\\DC4");
            }
            else if (0x15 == b)
            {
                temp += QString("\\NAK");
            }
            else if (0x16 == b)
            {
                temp += QString("\\SYN");
            }
            else if (0x17 == b)
            {
                temp += QString("\\ETB");
            }
            else if (0x18 == b)
            {
                temp += QString("\\CAN");
            }
            else if (0x19 == b)
            {
                temp += QString("\\EM");
            }
            else if (0x1A == b)
            {
                temp += QString("\\SUB");
            }
            else if (0x1B == b)
            {
                temp += QString("\\ESC");
            }
            else if (0x1C == b)
            {
                temp += QString("\\FS");
            }
            else if (0x1D == b)
            {
                temp += QString("\\GS");
            }
            else if (0x1E == b)
            {
                temp += QString("\\RS");
            }
            else if (0x1F == b)
            {
                temp += QString("\\US");
            }
            else if (0x7F == b)
            {
                temp += QString("\\x7F");
            }
            else if (0x5C == b)
            {
                temp += QString("\\x5C");
            }
            else if (0x20 >= b)
            {
                temp += QString("\\x%1").arg(decimalToStrHex((quint8)b));
            }
            else
            {
                temp += QString("%1").arg(b);
            }
        }

        return temp.trimmed();
    }

    static void fillStringToCharBuffer(const QString &str, uchar *buf, int size)
    {
        const uchar *strchar = (const uchar *)str.toLocal8Bit().data();
        int strsize = str.size();
        int nn = 0, kk = 0;
        int copysize = qMin(strsize, size);

        for (nn = 0; nn != copysize; nn++)
        {
            buf[kk++] = strchar[nn];
        }

        if (copysize < size)
        {
            for (nn = 0; nn != (size - copysize); nn++)
            {
                buf[kk++] = 0x0;
            }
        }
    }

    //!
    //! \brief split 分割字符串
    //! \param s: 原字符串
    //! \param tokens : 存储分割后字符串列表
    //! \param delimiters: 分隔符
    //!
    static void split(const std::string &s, std::vector<std::string> &tokens, const std::string &delimiters)
    {
        std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
        std::string::size_type pos = s.find_first_of(delimiters, lastPos);
        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            tokens.push_back(s.substr(lastPos, pos - lastPos));  //use emplace_back after C++11
            lastPos = s.find_first_not_of(delimiters, pos);
            pos = s.find_first_of(delimiters, lastPos);
        }
    }

    static bool endsWith(const std::string &str, const std::string &suffix)
    {
        return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
    }

    static bool startsWith(const std::string &str, const std::string &prefix)
    {
        return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
    }

    //QMap运算符[]替代接口，值获取失败抛出异常
    template <typename T1, typename T2>
    inline static const T2 mapValue(const QMap<T1, T2> &map, const T1 &key)
    {
        if (!map.contains(key))
        {
            QString errormsg = QString("map not contain the key:%1").arg(key);
            throw std::runtime_error(errormsg.toStdString());
        }

        return map[key];
    }

    //计算时间差
    static int calculate_elapsed_mins(const TimeSpecCross &tm)
    {
#ifdef Q_OS_WIN
        QDateTime nowtm = QDateTime::currentDateTime();
        return tm.secsTo(nowtm) / 60;
#else
        struct timespec nowtm = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &nowtm);
        return (nowtm.tv_sec - tm.tv_sec) / 60;
#endif
    }

    static qint64 calculate_elapsed_secs(const TimeSpecCross &tm)
    {
#ifdef Q_OS_WIN
        QDateTime nowtm = QDateTime::currentDateTime();
        return tm.secsTo(nowtm);
#else
        struct timespec nowtm = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &nowtm);
        return (nowtm.tv_sec - tm.tv_sec);
#endif
    }

    static qint64 calculate_elapsed_msecs(const TimeSpecCross &last_tm, const TimeSpecCross &now_tm)
    {
#ifdef Q_OS_WIN
        return last_tm.secsTo(now_tm);
#else
        qint64 diff = (now_tm.tv_sec - last_tm.tv_sec) * 1000;
        diff += (now_tm.tv_nsec - last_tm.tv_nsec) / 1000000;
        return diff;
#endif
    }

    //生成时间戳
    static TimeSpecCross generaTimeStamp()
    {
#ifdef Q_OS_WIN
        return QDateTime::currentDateTime();
#else
        struct timespec nowtm = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &nowtm);
        return nowtm;
#endif
    }

    //获取当前时间isodate格式时间戳
    static QString get_current_isodate_timestamp(void)
    {
        return QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
    }

    //系统重启
    static bool sysReboot(void)
    {
        qWarning() << "sysReboot...";
#ifdef __ARM_EABI__

        QString cmd = QString("sleep 3s; /sbin/reboot");
        FILE *fp = ::popen(cmd.toStdString().c_str(), "r");
        if (!fp)
        {
            ELOG_WARN(::strerror(errno));
            return (false);
        }
        ::pclose(fp);
#endif
        return (true);
    }

    //线程关闭
    static void threadQuit(QThread *thr, const QString &flag)
    {
        if (thr->isRunning())
        {
            thr->quit();
            if (!thr->wait(3000))
            {
                qWarning() << QString("强制杀死%1线程").arg(flag).toStdString().c_str();
                thr->terminate();
                qWarning() << QString("%1线程已杀死").arg(flag).toStdString().c_str();
            }
        }
    }

    //是否为远程url
    static bool is_remote_url(const QString &url)
    {
        QStringList urllist = url.split(QString("://"));

        return (urllist.size() < 2) ? false : true;
    }

    //校验ip地址合法性-排除特殊用途ip地址
    //true=正常ip地址
    static bool check_normal_ip(const QString &ip)
    {
        // 格式 a.b.c.d
        //169.254.x.x
        //127.0.0.0
        //224.x.x.x
        //240.x.x.x
        //255.255.255.255  不可用ip
        bool isok = false;
        QStringList tmplist = ip.split(QString("."));
        if (tmplist.size() != 4) return false;

        int a = tmplist.at(0).toInt(&isok);
        if (!isok) return false;

        int b = tmplist.at(1).toInt(&isok);
        if (!isok) return false;

        int c = tmplist.at(2).toInt(&isok);
        if (!isok) return false;

        int d = tmplist.at(3).toInt(&isok);
        if (!isok) return false;

        if ((a <= 0) || (a > 255) || (b < 0) || (b > 255) || (c < 0) || (c > 255) || (d < 0) || (d > 255)) return false;

        if (a == 169 && b == 254) return false;
        if (a == 127 && b == 0 && c == 0 && d == 0) return false;
        if (a == 224 || a == 240) return false;
        if (a == 255 && b == 255 && c == 255 && d == 255) return false;

        return true;
    }

    //! 检查子网掩码合法性
    static bool check_normal_netmask(const QString &netmask)
    {
        //先验证是否为合法IP，然后将掩码转化成32无符号整型，取反为000...00111...1，然后再加1为00...01000...0，此时为2^n，如果满足就为合法掩码
        struct in_addr addr;
        int ret = inet_pton(AF_INET, netmask.toStdString().c_str(), &addr);
        if (ret)
        {
            unsigned int b = 0, i, n[4];
            sscanf(netmask.toStdString().c_str(), "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);
            for (i = 0; i < 4; ++i)  //将子网掩码存入32位无符号整型
                b += n[i] << (i * 8);
            b = ~b + 1;
            if ((b & (b - 1)) == 0)  //判断是否为2^n
                return true;
        }

        return false;
    }

    //!快速求log2N对数
    static int fast_log2(int n)
    {
        int result = 0;
        if (n & 0xffff0000)
        {
            result += 16;
            n >>= 16;
        }
        if (n & 0x0000ff00)
        {
            result += 8;
            n >>= 8;
        }
        if (n & 0x000000f0)
        {
            result += 4;
            n >>= 4;
        }
        if (n & 0x0000000c)
        {
            result += 2;
            n >>= 2;
        }
        if (n & 0x00000002)
        {
            result += 1;
            n >>= 1;
        }
        return result;
    }

    //!保存文件内容(原内容会被清空）
    static bool savefile(const std::string &data, const std::string &path)
    {
        bool ret = false;
        std::ofstream os(path, ios_base::out | ios_base::binary | ios_base::trunc);
        if (os)
        {
            ret = true;
            os << data;
            os.flush();
        }

        os.close();

#ifdef Q_OS_LINUX
        ::sync();  //清空系统缓冲区，强制将缓冲区数据写入磁盘
#endif
        return ret;
    }

    static bool savefile(const char *data, int datasize, const std::string &path)
    {
        bool ret = false;
        std::ofstream os(path, ios_base::out | ios_base::binary | ios_base::trunc);
        if (os)
        {
            ret = true;
            os.write(data, datasize);
            os.flush();
        }

        os.close();

#ifdef Q_OS_LINUX
        ::sync();  //清空系统缓冲区，强制将缓冲区数据写入磁盘
#endif

        return ret;
    }

    //!
    //! \brief setSysNetwork 修改系统网络参数
    //! \param ip
    //! \param netmask
    //! \param gateway
    //!
    static void setSysNetwork(const std::string &ip, const std::string &netmask, const std::string &gateway)
    {
        //    [root@dd259]:dd259$: cat /etc/network/interfaces
        //    auto eth0
        //    iface eth0 inet static
        //    address 192.168.19.94
        //    netmask 255.255.255.0
        //    gateway 192.168.19.1
        //    dns-nameservers 192.168.19.1

        Q_UNUSED(ip)
        Q_UNUSED(netmask)
        Q_UNUSED(gateway)
#ifdef __ARM_EABI__
        std::string str;
        str = std::string("auto eth0\n");
        str += std::string("iface eth0 inet static\n");
        str += std::string("address ") + ip + std::string("\n");
        str += std::string("netmask ") + netmask + std::string("\n");
        str += std::string("gateway ") + gateway + std::string("\n");
        str += std::string("dns-nameservers ") + gateway + std::string("\n");

        savefile(str, std::string("/etc/network/interfaces"));

        //使修改网络参数生效
        netif_restart();
#endif
    }

    //!检测网络参数是否与配置文件一致
    //! true=一致 false=不一致
    static bool checkSysNetwork(const std::string &ip, const std::string &netmask, const std::string &gateway)
    {
        Q_UNUSED(ip)
        Q_UNUSED(netmask)
        Q_UNUSED(gateway)

#ifdef __ARM_EABI__
        bool result = false;
        std::ifstream in(std::string("/etc/network/interfaces"), std::ios_base::in | std::ios_base::binary);
        if (in)
        {
            int comparecnt = 0;
            std::string line;
            while (std::getline(in, line))
            {
                //    auto eth0
                //    iface eth0 inet static
                //    address 192.168.19.94
                //    netmask 255.255.255.0
                //    gateway 192.168.19.1
                //    dns-nameservers 192.168.19.1

                if (startsWith(line, std::string("address")))
                {
                    std::vector<std::string> tokens;
                    split(line, tokens, std::string(" "));
                    if ((tokens.size() >= 2) && (tokens.at(1) == ip)) comparecnt++;
                }

                if (startsWith(line, std::string("netmask")))
                {
                    std::vector<std::string> tokens;
                    split(line, tokens, std::string(" "));
                    if ((tokens.size() >= 2) && (tokens.at(1) == netmask)) comparecnt++;
                }

                if (startsWith(line, std::string("gateway")))
                {
                    std::vector<std::string> tokens;
                    split(line, tokens, std::string(" "));
                    if ((tokens.size() >= 2) && (tokens.at(1) == gateway)) comparecnt++;
                }

                if (comparecnt == 3)
                {
                    result = true;
                    break;
                }
            }

            in.close();
        }

        return result;
#else
        return true;
#endif
    }

    //! 同步
    static void sync()
    {
#ifdef Q_OS_LINUX
        ::sync();  //清空系统缓冲区，强制将缓冲区数据写入磁盘
#endif
    }

    //!重启网卡
    static void netif_restart()
    {
#ifdef __ARM_EABI__
        shellexec("/etc/init.d/S40network restart");
#endif
    }

    //!校时
    static void setSysTime(const QDateTime &tm)
    {
        Q_UNUSED(tm)
#ifdef __ARM_EABI__
        //date -s “2008-05-23 01:01:01″
        QStringList args;
        QString tmstr = tm.toString("yyyy-MM-dd hh:mm:ss");
        args << "-s" << tmstr;  //QString("%1").arg(tmstr);
        //qDebug()<<args;
        QProcess::execute("/bin/date", args);
#ifdef __ARM_EABI__
        args.clear();
        args << "-wu";
        QProcess::startDetached(QString("/sbin/hwclock"), args);  // 同步系统时间
//        QProcess::startDetached("sleep 5s && sync");              // 保存配置
#endif
#endif
    }

    //获取网卡设备ipv4地址
    static QString getNetWorkIPv4Address()
    {
#ifdef __ARM_EABI__
        QNetworkInterface iface = QNetworkInterface::interfaceFromName(QString("eth0"));
#else
        QNetworkInterface iface = QNetworkInterface::interfaceFromName(QString("enp3s0"));
#endif
        QString ipv4address;

        if (!(iface.flags() & (QNetworkInterface::IsLoopBack | QNetworkInterface::IsPointToPoint)))
        {
            QList<QHostAddress> ip_addresses = iface.allAddresses();
            foreach (const QHostAddress &tmpaddress, ip_addresses)
            {
                bool expire_address = true;
                if (tmpaddress == QHostAddress::Null || tmpaddress == QHostAddress::LocalHost || tmpaddress == QHostAddress::LocalHostIPv6 ||
                    tmpaddress == QHostAddress::Broadcast || tmpaddress == QHostAddress::Any || tmpaddress == QHostAddress::AnyIPv6)
                {
                    continue;
                }

                if (expire_address && (tmpaddress.protocol() == QAbstractSocket::IPv4Protocol))
                {
                    ipv4address = tmpaddress.toString();
                    break;
                }
            }
        }

        return ipv4address;
    }

    //获取硬件MAC
    static QString getHWAddress(void)
    {
        QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();

        for (int nn = 0; nn != nets.size(); nn++)
        {
            if (nets.at(nn).name().startsWith("e"))
            {
                return nets.at(nn).hardwareAddress();
            }
        }
        qDebug() << "获取硬件MAC地址失败!";
        return QString("00:00:00:00:00:00:");
    }

    //获取系统最近重启时间
    static QDateTime getLastRebootTime(void)
    {
        QDateTime nowdt = QDateTime::currentDateTime();

        QFile uptimefi(QString("/proc/uptime"));
        if (uptimefi.open(QIODevice::ReadOnly))
        {
            bool is_ok = true;
            QString uptmcontent = QString(uptimefi.readAll());
            QString uptime = uptmcontent.split(QString(" ")).first().trimmed();
            double upseconds = uptime.toDouble(&is_ok);
            if (is_ok)
            {
                qint64 upms = 0 - (qint64)(upseconds * 1000);
                nowdt = nowdt.addMSecs(upms);
            }
            uptimefi.close();
        }

        return nowdt;
    }

    //获取系统CPU温度
    static int getSystemCPUTemp(void)
    {
        int nowtemp;
        bool isok = false;
        int cputemp = 30;

        QFile cputempfi("/sys/class/thermal/thermal_zone0/temp");
        if (cputempfi.open(QIODevice::ReadOnly))
        {
            QString tempstr = cputempfi.readAll();
            nowtemp = tempstr.toInt(&isok);
            if (isok)
            {
                cputemp = (nowtemp / 1000) - 40 + 35;
            }

            cputempfi.close();
        }

        return cputemp;
    }

    static int lockfile(int fd)
    {
        Q_UNUSED(fd)
#ifdef Q_OS_LINUX
        struct flock fl;

        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;

        return (::fcntl(fd, F_SETLK, &fl));
#else
        return 0;
#endif
    }

    //单例运行
    static int already_running(const char *filename)
    {
        Q_UNUSED(filename)
#ifdef Q_OS_LINUX
        int fd;
        int ret = 0;
        char buf[16];

        fd = ::open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd < 0)
        {
            ::printf("error:can't open %s: %s\n", filename, strerror(errno));
            ::exit(1);
        }
        if (lockfile(fd) == -1)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                ::printf("error: file: %s already locked\n", filename);
                ::close(fd);
                return 1;
            }
            ::printf("error: can't lock %s: %s\n", filename, strerror(errno));
            ::exit(1);
        }
        ret = ::ftruncate(fd, 0);
        ::sprintf(buf, "%ld", (long)::getpid());
        ret = ::write(fd, buf, ::strlen(buf) + 1);

        Q_UNUSED(ret)
#endif
        return 0;
    }

    //屏蔽信号
    static void blockSystemSignal()
    {
#ifdef Q_OS_LINUX
        sigset_t newMask, oldMask;
        //信号屏蔽
        /*block SIGINT*/
        ::sigemptyset(&newMask);
        ::sigaddset(&newMask, SIGINT);
        //::sigaddset(&newMask, SIGPIPE);
        //        ::sigaddset(&newMask, SIGALRM);//屏蔽该信号会导致QProcess 不能正常退出 发送finished error 信号
        ::sigaddset(&newMask, SIGUSR1);
        ::sigaddset(&newMask, SIGUSR2);

        if (::sigprocmask(SIG_BLOCK, &newMask, &oldMask) < 0)
        {
            ::perror("sigprocmask error");
        }
#endif
    }

    //!获取compare_dt时间距离base_dt时间的秒数，>0表示compare_dt>base_dt
    static qint64 get_secs_from(const QDateTime &compare_dt, const QDateTime &base_dt)
    {
        qint64 base_secs = base_dt.toMSecsSinceEpoch() / 1000;
        qint64 compare_secs = compare_dt.toMSecsSinceEpoch() / 1000;

        return (compare_secs - base_secs);
    }

    //!获取时间d1与d2之间的秒数差值
    static quint64 get_secs_diff(const QDateTime &d1, const QDateTime &d2)
    {
        qint64 diff = get_secs_from(d1, d2);

        return (qAbs(diff));
    }
#ifdef Q_OS_LINUX
    static void InstallSigUSR1ExitHandle(sighandler_t handler)
    {
        ::signal(SIGUSR1, handler);
        ::signal(SIGTERM, handler);
        ::signal(SIGKILL, handler);
    }
#else
    static void InstallSigUSR1ExitHandle(void *handler)
    {
        Q_UNUSED(handler)
    }
#endif

    //!
    //! \brief process_search_by_name 搜索进程是否存在
    //! \param procName : 进程名
    //! \return  true=存在 false=不存在
    //!
    static bool process_search_by_name(const QString &procName)
    {
        QStringList args;
        args << "-f" << procName;
        int result = QProcess::execute(QString("pgrep"), args);
        return result == 0;
    }

    //!
    //! \brief shellexec 等待命令执行完成
    //! \param cmd
    //! \return
    //!执行shell命令
    static bool shellexec(const QString &cmd)
    {
        Q_UNUSED(cmd)
//        qDebug() << "shellexe: " << cmd;
#ifdef Q_OS_LINUX
#if 0
        int rc = 0;  // 用于接收命令返回值
        char result_buf[1024];

        //if(cmd.startsWith(QString("cp"))) {
        //    ELOG_WARN("shellexec={:s}",cmd.toStdString());
        //}

        FILE *fp = ::popen(cmd.toStdString().c_str(), "r");
        if (!fp)
        {
            ELOG_WARN("kk{:s}:{:s}", cmd.toStdString().c_str(), ::strerror(errno));
            return (false);
        }
        //读取管道输出否则返回结果不正确
        while (::fgets(result_buf, sizeof(result_buf), fp) != NULL)
            ;

        rc = ::pclose(fp);
        if (-1 == rc)
        {
            ELOG_WARN("关闭文件指针失败:{:s}", ::strerror(errno));
            return false;
        }
        //ELOG_WARN("{:s} return={:d}",cmd.toStdString(),WEXITSTATUS(rc));
        return (WEXITSTATUS(rc) == 0) ? true : false;
#else
        //extern "C" {
        //#include <sys/types.h>
        //#include <unistd.h>
        //#include <sys/wait.h>
        //}
        //linux c调用shell命令，相较于system和popen不会造成频繁调用程序卡死

        int pid = 0;
        int status = 0;
        const char *argv[4];
        const char *new_environ[] = {NULL};

        if (cmd.isEmpty()) return false;

        pid = vfork();
        if (pid < 0) return false;

        if (0 == pid)
        { /* child process */
            argv[0] = "sh";
            argv[1] = "-c";
            argv[2] = cmd.toLocal8Bit().data();
            argv[3] = NULL;

            execve("/bin/sh", (char *const *)argv, (char *const *)new_environ); /* execve() also an implementation of exec() */
            exit(127);
        }

        //等待子进程命令执行结束
        waitpid(pid, &status, 0);

        return WIFEXITED(status) ? true : false;
#endif
#else
        return true;
#endif
    }

    static QDateTime getBuildTime()
    {
        static const QDate buildDate = QLocale(QLocale::English).toDate(QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
        static const QTime buildTime = QTime::fromString(__TIME__, "hh:mm:ss");

        //return QString("%1%2").arg(buildDate.toString("yyyyMMdd")).arg(buildTime.toString("hhmmss"));
        return QDateTime(buildDate, buildTime);
    }

    //检查网络接口连接状态
    //0=DOWN 1=UP 2=ERROR
    static int net_detect(const QString &net_name)
    {
        Q_UNUSED(net_name)
#ifdef Q_OS_LINUX
        int skfd = 0, net_status = 2;
        struct ifreq ifr;

        skfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (skfd < 0)
        {
            qDebug() << "<net_detect> Open socket error!\n";
            return 2;
        }

        strcpy(ifr.ifr_name, net_name.toStdString().c_str());

        if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
        {
            qDebug() << "<net_detect> ioctl:{:s}", strerror(errno);
        }
        else
        {
            if (ifr.ifr_flags & IFF_RUNNING)
            {
                net_status = 1;
            }
            else
            {
                net_status = 0;
            }
        }

        close(skfd);

        return net_status;
#else
        return 1;
#endif
    }

    //! 使用TCP Keep-alive加上TCP_USER_TIMEOUT机制，就可以完美解
    //! 决通信对端异常断网、掉电的情况下，连接被长期挂起的问题

    //! 配置tcp保活
    //! enable: 1=开启
    //! idletime: 空闲时间，单位秒，在该时间内若无通信数据则发送保活探测包
    //! intervaltime:保活包发送间隔，单位秒
    //! checkcount:保活包无应答最大发送次数
    static bool setTcpSocketKeepAlive(int sockfd, int enable, int ildetime, int intervaltime, int checkcount)
    {
        Q_UNUSED(sockfd)
        Q_UNUSED(enable)
        Q_UNUSED(ildetime)
        Q_UNUSED(intervaltime)
        Q_UNUSED(checkcount)
#ifdef Q_OS_LINUX
        if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&enable, sizeof(enable)) < 0)
        {
            qDebug() << "setsockopt: SO_KEEPALIVE failed:{:s}", ::strerror(errno);
            return false;
        }

        if (setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&ildetime, sizeof(ildetime)) < 0)
        {
            qDebug() << "setsockopt: TCP_KEEPIDLE failed:{:s}", ::strerror(errno);
            return false;
        }

        if (setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&intervaltime, sizeof(intervaltime)) < 0)
        {
            qDebug() << "setsockopt: TCP_KEEPINTVL failed:{:s}", ::strerror(errno);
            return false;
        }

        if (setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&checkcount, sizeof(checkcount)) < 0)
        {
            qDebug() << "setsockopt: TCP_KEEPCNT failed:{:s}", ::strerror(errno);
            return false;
        }
#endif
        return true;
    }

    //! 数据包被发送后未接收到ACK确认的最大时长，以毫秒
    //! 为单位，例如设置为timeout时，代表如果发送出去的数据包
    //! 在timeout毫秒内未收到ACK确认，则下一次调用send或者recv，
    //! 函数会返回-1，errno设置为ETIMEOUT，代表connection timeout
    static bool setTcpAckTimeoutMs(int socketfd, int timeout_ms)
    {
        Q_UNUSED(socketfd)
        Q_UNUSED(timeout_ms)
#ifdef Q_OS_LINUX
        if (setsockopt(socketfd, IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout_ms, sizeof(timeout_ms)) < 0)
        {

            qDebug() << "setsockopt: TCP_USER_TIMEOUT failed:{:s}", ::strerror(errno);
            return false;
        }
#endif
        return true;
    }

    static void getCharRandom(unsigned char *buf, int buflen, unsigned char minvalue, unsigned char maxvalue)
    {
        int nn = 0;
        static std::random_device rd;
        static std::mt19937 e(rd());
        static std::uniform_int_distribution<unsigned char> gen(minvalue, maxvalue);

        for (nn = 0; nn < buflen; nn++) buf[nn] = gen(e);
    }

    //CRC16-RTU校验数值计算
    //初始值 0xFFFF 多项式: 0xa001
    static unsigned short crc16_rtu_calculate(unsigned char *data, unsigned int len)
    {
        unsigned int ii, jj;
        unsigned int crc;
        unsigned char chk;

        crc = 0xFFFF;

        for (ii = 0; ii < len; ii++)
        {
            crc = crc ^ data[ii];

            for (jj = 0; jj < 8; jj++)
            {
                chk = crc & 1;
                crc = crc >> 1;
                crc = crc & 0x7fff;

                if (chk == 1)
                {
                    crc = crc ^ 0xa001;
                }
                //crc = crc & 0xffff;
            }
        }

        return crc;
    }

    //!转换指定精度浮点数
    template <typename T>
    static std::string to_string_with_precision(const T a_value, const int n = 6)
    {
        int nn = n;
        std::ostringstream out;
        out.setf(ios_base::showpoint);
        out << std::fixed << std::setprecision(nn) << a_value;  //
        return out.str();
    }

    //!转换浮点数,指定小数点后非0部分有效数字位数
    template <typename T>
    static std::string to_string_with_precision_none_zero(const T a_value, const int n = 6)
    {
        unsigned long long v0 = std::fabs(a_value);
        T vf = a_value - v0;
        int f0cnt = 0;

        if (v0 > 0)
        {
            f0cnt = n;
        }
        else
        {
            f0cnt = 0;
            T diff0 = std::numeric_limits<T>::epsilon() + 1.0;

            do {
                vf = vf * 10;
                f0cnt++;
                if (vf > diff0)
                {
                    f0cnt += n;
                    break;
                }

                if (f0cnt > 12)
                {
                    f0cnt = 6;
                    break;
                }

            } while (1);
        }

        std::ostringstream out;
        out.setf(ios_base::showpoint);
        out << std::fixed << std::setprecision(f0cnt) << a_value;
        return out.str();
    }

    ///////////////////////////////////////////
    //!日志初始化
    static void logger_init(const QString &apptag)
    {
        //eloggerwrapper::logger_init(apptag.toStdString());
    }

    //!设置日记级别
    //! 0=trace 1=debug 2=info 3=warn 4=err 5=critical 6=off
    static void logger_setlevel(uint level)
    {
        //eloggerwrapper::logger_setlevel(level);
    }

    //!设置日志格式是否添加文件信息
    static void logger_set_format_with_fileinfo(bool en)
    {
        //eloggerwrapper::logger_set_format_with_fileinfo(en);
    }

    //!立即刷新日志
    static void logger_flush()
    {

        //eloggerwrapper::logger_flush();
    }

    //!日志关闭
    static void logger_close()
    {
    }

    ///////////////////////////////////////////

    static void msleep(unsigned long msec)
    {
        QMutex mutex;
        QWaitCondition sleep;
        mutex.lock();
        sleep.wait(&mutex, msec);
        mutex.unlock();
    }

private:
};

#endif  // HELPTOOLS_H
