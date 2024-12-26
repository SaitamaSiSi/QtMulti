#include "keyverify.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <iostream>
#include <string>

#include "base64helper.h"
#include "keywarnout.h"
#include "rsahelper.h"

#define GJKTOUCH_CPU_S5P4418

namespace monocypher {
//    extern "C" {
#include "monocypher.h"
    //    }
}  // namespace monocypher

//USING_NAMESPACE(CryptoPP)
using namespace std;

/// @brief ECID Module's Register List
/// Base Addr: 0xC0067000
struct NX_ECID_Reg {
    volatile unsigned int ECID[4];        ///< 0x00 ~ 0x0C    : 128bit ECID Register
    volatile unsigned char CHIPNAME[48];  ///< 0x10 ~ 0x3C    : Chip Name Register
    volatile unsigned int RESERVED;       ///< 0x40           : Reserved Region
    volatile unsigned int GUID0;          ///< 0x44           : GUID 0 Register
    volatile unsigned short GUID1;        ///< 0x48           : GUID 1 Register
    volatile unsigned short GUID2;        ///< 0x4A           : GUID 2 Register
    volatile unsigned char GUID3[8];      ///< 0x4C ~ 0x50    : GUID 3-0 ~ 3-7 Register
    volatile unsigned int EC[3];          ///< 0x54 ~ 0x5C    : EC 0 ~ 3 Register
};

////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////

KeyVerify::KeyVerify(QObject *parent)
    : QObject(parent)
{
}

QByteArray KeyVerify::GetIDString2(bool &result)
{
    QByteArray idbytes = GetIDString(result);

    if (result)
    {
        //加密
        unsigned char hash[16];
        unsigned char message[32];
        unsigned char key[32] = {
            0x94, 0xae, 0x59, 0x4b, 0xa9, 0x2c, 0x1e, 0x92, 0xc3, 0x0d, 0xc6, 0x54, 0xe5, 0x68, 0xda, 0x20,
            0xc9, 0x44, 0xa4, 0x25, 0x5a, 0xf9, 0xea, 0xe2, 0xfc, 0xcc, 0x09, 0x6b, 0x88, 0xec, 0xb1, 0x06};

        ::memcpy(message, idbytes.data(), idbytes.size());

        monocypher::crypto_blake2b_general(hash, 16, key, 32, message, 32);
        /* Wipe secrets if they are no longer needed */
        monocypher::crypto_wipe(message, idbytes.size());
        monocypher::crypto_wipe(key, 32);
        idbytes.clear();
        idbytes.reserve(16);
        for (int nn = 0; nn != 16; nn++)
        {
            idbytes.append((char)hash[nn]);
        }

        return idbytes;
    }

    return QByteArray();
}

bool KeyVerify::Encryp(const QString &keyOutPath, const QString &cpuidpath)
{
    bool result;
    QByteArray OrigData;

    if (cpuidpath.isEmpty())
    {
        OrigData = GetIDString(result);
        if (!result) return false;
    }
    else
    {
        OrigData = GetIDStringFromFile(cpuidpath, result);
        if (!result) return false;
    }

    QByteArray seq0 = OrigData.mid(0, 6);
    QByteArray seq1 = OrigData.mid(6, 7);
    QByteArray seq2 = OrigData.mid(13);

    qsrand(QDateTime::currentMSecsSinceEpoch());
    QByteArray gr0 = GetGanRaoBytes(0, 17);
    QByteArray gr1 = GetGanRaoBytes(0, 9);
    QByteArray gr2 = GetGanRaoBytes(0, 20);
    QByteArray gr3 = GetGanRaoBytes(0, 41);

    QByteArray encypData = gr0 + seq0 + gr1 + seq1 + gr2 + seq2 + gr3;
    return Encryp_(encypData, keyOutPath);
}

bool KeyVerify::Encryp_(QByteArray &origdata, const QString &keyOutPath)
{
    QByteArray encrypData = EncrypPriv(origdata);
    if (encrypData.isEmpty()) return false;

    QFile keyfile(keyOutPath);
    if (!keyfile.open(QIODevice::WriteOnly))
        return false;
    keyfile.resize(0);
    keyfile.write(encrypData);
    keyfile.close();
    return true;
}

bool KeyVerify::Verify(const QString &keyPath, const QString &cpuidpath)
{
    bool result;
    QByteArray OrigData;

    if (cpuidpath.isEmpty())
    {
        OrigData = GetIDString(result);
        if (!result) return false;
    }
    else
    {
        OrigData = GetIDStringFromFile(cpuidpath, result);
        if (!result) return false;
    }

    QFile keyfile(keyPath);

    if (!keyfile.open(QIODevice::ReadOnly))
    {
        KeyWarnOut::out("软件许可证文件打开失败!");
        return false;
    }
    QByteArray DecryData = DecrypPriv(keyfile.readAll());
    keyfile.close();

    QByteArray seq0 = DecryData.mid(17, 6);
    QByteArray seq1 = DecryData.mid(32, 7);
    QByteArray seq2 = DecryData.mid(59, 3);
    QByteArray iddata = seq0 + seq1 + seq2;

    if (QString(OrigData) == QString(iddata))
        return true;

    KeyWarnOut::out("软件许可证验证错误!");
    return false;
}

QByteArray KeyVerify::EncrypPriv(const QByteArray &Origdata)
{
#if 0
    QFile ndatfile(QString(":/key/Nfactor.dat"));
    if (!ndatfile.open(QIODevice::ReadOnly)) return QByteArray();
    QByteArray ndat = ndatfile.readAll();
    string ndatstr;
    StringSource(ndat.data(), true, new Base64Decoder(new StringSink(ndatstr)));
    ndatfile.close();
    //QByteArray ndat2 = QByteArray()
    ndatstr = string("0x") + ndatstr.substr(18, 512);

    QFile ddatfile(QString(":/key/Dfactor.dat"));
    if (!ddatfile.open(QIODevice::ReadOnly)) return QByteArray();
    QByteArray ddat = ddatfile.readAll();
    string ddatstr;
    StringSource(ddat.data(), true, new Base64Decoder(new StringSink(ddatstr)));
    ddatfile.close();
    ddatstr = string("0x") + ddatstr.substr(42, 512);

    Integer n(ndatstr.c_str());
    Integer d(ddatstr.c_str());

    CryptoPP::RSA::PublicKey pubKey_;
    pubKey_.Initialize(n, d);

    AutoSeededRandomPool rng;

    string cipher;
    RSAES_OAEP_SHA_Encryptor pub(pubKey_);
    StringSource(Origdata.data(), true, new PK_EncryptorFilter(rng, pub, new HexEncoder(new StringSink(cipher))));

    //base64加密
    string enc64;
    StringSource(cipher, true, new Base64Encoder(new StringSink(enc64)));

    return QByteArray(enc64.c_str());
#else
    //解密密钥文件
    QByteArray keycontext = decryptRSAKeybyxchacha(QString(":/key/rsa_pub_encrypt_key"));
    int encryptlen = RSAHelper::calcDataEncryptLegth(Origdata.size(), 3072);
    int finallen = 0;
    QByteArray finalkeycontext;
    char *tmpclipher = new char[encryptlen];
    finallen = RSAHelper::pubEncrypt(tmpclipher, Origdata.data(), Origdata.size(), keycontext.data(), keycontext.size());
    if (finallen > 0)
    {
        finalkeycontext = Base64Helper::encode(QByteArray(tmpclipher, finallen));  //QByteArray(tmpclipher, finallen).toBase64();
    }
    delete[] tmpclipher;

    return finalkeycontext;
#endif
}

QByteArray KeyVerify::DecrypPriv(const QByteArray &encrypData)
{
#if 0
    string dec64;
    StringSource(encrypData.data(), true, new Base64Decoder(new StringSink(dec64)));

    QFile ndatfile(QString(":/key/Nfactor.dat"));
    if (!ndatfile.open(QIODevice::ReadOnly)) return QByteArray();
    QByteArray ndat = ndatfile.readAll();
    string ndatstr;
    StringSource(ndat.data(), true, new Base64Decoder(new StringSink(ndatstr)));
    ndatfile.close();
    ndatstr = string("0x") + ndatstr.substr(18, 512);

    QFile ddatfile(QString(":/key/Dfactor.dat"));
    if (!ddatfile.open(QIODevice::ReadOnly)) return QByteArray();
    QByteArray ddat = ddatfile.readAll();
    string ddatstr;
    StringSource(ddat.data(), true, new Base64Decoder(new StringSink(ddatstr)));
    ddatfile.close();
    ddatstr = string("0x") + ddatstr.substr(42, 512);

    QFile edatfile(QString(":/key/Efactor.dat"));
    if (!edatfile.open(QIODevice::ReadOnly)) return QByteArray();
    QByteArray edat = edatfile.readAll();
    string edatstr;
    StringSource(edat.data(), true, new Base64Decoder(new StringSink(edatstr)));
    edatfile.close();
    edatstr = string("0x") + edatstr.substr(23, 5);

    Integer n(ndatstr.c_str());
    Integer d(ddatstr.c_str());
    Integer e(edatstr.c_str());

    AutoSeededRandomPool rng;

    CryptoPP::RSA::PrivateKey privKey_;
    privKey_.Initialize(n, d, e);
    RSAES_OAEP_SHA_Decryptor priv(privKey_);

    string recovered;
    try
    {
        StringSource(dec64, true, new HexDecoder(new PK_DecryptorFilter(rng, priv, new StringSink(recovered))));
    } catch (...)
    {
        recovered.clear();
    }

    return QByteArray(recovered.c_str());
#else
    QByteArray keycontext = decryptRSAKeybyxchacha(QString(":/key/rsa_pri_encrypt_key"));
    QByteArray cliphercontext = Base64Helper::decode(encrypData);  //QByteArray::fromBase64(encrypData);
    char *tmpplain = new char[cliphercontext.size()];
    int plainlen = RSAHelper::priDecrypt(tmpplain, cliphercontext.data(), cliphercontext.size(), keycontext.data(), keycontext.size());
    QByteArray plaincontext(tmpplain, plainlen);
    delete[] tmpplain;
    return plaincontext;
#endif
}

QByteArray KeyVerify::GetIDString(bool &result)
{
#ifdef DD301_CPU_H3
    //    hexdump /sys/bus/nvmem/devices/sunxi-sid0/nvmem
    //    0000000 8082 0447 0064 04c3 3650 ce0a 1e28 2202
    //    0000010 0002 0000 0000 0000 0000 0000 0000 0000
    //    0000020 0000 0000 0000 0000 0000 0000 0000 0000
    //    0000030 0000 0008 0508 0000 0000 0000 0000 0000
    //    0000040 0000 0000 0000 0000 0000 0000 0000 0000
    QFile fi(QString("/sys/bus/nvmem/devices/sunxi-sid0/nvmem"));
    QByteArray idbytes;
    idbytes.resize(16);
    idbytes.fill((char)0xFF);

    if (!fi.open(QIODevice::ReadOnly))
    {
        result = false;
        return idbytes;
    }

    QDataStream outs(&fi);

    if (!outs.readRawData(idbytes.data(), idbytes.size()))
    {
        result = false;
        return idbytes;
    }

    result = true;

    //qDebug()<<"Chip ID="<<idbytes.toHex().toUpper();
    return idbytes.toHex().toUpper();

#elif defined DD301_CPU_A40i
    //    cat /sys/class/sunxi_info/sys_info

    //    sunxi_platform    : sun8iw11p1
    //    sunxi_secure      : normal
    //    sunxi_chipid      : 2004085178e0891c000004c000000000
    //    sunxi_chiptype    : 00000001
    //    sunxi_batchno     : 0x1

    QFile fi(QString("/sys/class/sunxi_info/sys_info"));

    QByteArray idbytes;
    idbytes.resize(16);
    idbytes.fill((char)0xFF);

    if (!fi.open(QIODevice::ReadOnly))
    {
        result = false;
        return idbytes;
    }

    QTextStream out(&fi);

    result = false;
    while (!out.atEnd())
    {
        QString linestr = out.readLine();
        if (linestr.startsWith(QString("sunxi_chipid")))
        {
            QStringList linelst = linestr.split(QString(":"));
            if (linelst.size() == 2)
            {
                QString chipidstr = linelst.at(1).trimmed();

                QByteArray chipidhex = hexStrToByteArray(chipidstr);
                idbytes.insert(0, chipidhex);
                idbytes.resize(16);
                result = true;
                break;
            }
            else
            {
                result = false;
            }
        }
    }

    if (result)
    {
        //加密
        unsigned char hash[16];
        unsigned char message[16];
        unsigned char key[32] = {
            0xED, 0x92, 0x02, 0xE6, 0x3F, 0x38, 0x30, 0x30, 0x55, 0xA2, 0x96, 0x2A, 0xE4, 0x03, 0x1F, 0x89,
            0x7A, 0x36, 0x32, 0x9E, 0x87, 0x0F, 0xF8, 0x63, 0x56, 0x32, 0x82, 0xA9, 0xB4, 0xB9, 0xB6, 0x53};

        ::memcpy(message, idbytes.data(), 16);

        monocypher::crypto_blake2b_general(hash, 16, key, 32, message, 16);
        /* Wipe secrets if they are no longer needed */
        monocypher::crypto_wipe(message, 16);
        monocypher::crypto_wipe(key, 32);

        for (int nn = 0; nn != 16; nn++)
        {
            idbytes[nn] = hash[nn];
        }
    }

    return idbytes;
#elif defined GJKTOUCH_CPU_S5P4418
    unsigned int ECID[4] = {0};
    int dev_fd;
    dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);
    if (dev_fd < 0)
    {
        KeyWarnOut::out("open(/dev/mem) failed.");
        result = false;
        return QByteArray(32, 0xFF);
    }
    size_t mapsize = sizeof(struct NX_ECID_Reg);
    struct NX_ECID_Reg *pNxECIDReg = (struct NX_ECID_Reg *)mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, 0xC0067000);
    ECID[0] = pNxECIDReg->ECID[0];
    ECID[1] = pNxECIDReg->ECID[1];
    ECID[2] = pNxECIDReg->ECID[2];
    ECID[3] = pNxECIDReg->ECID[3];

    if (dev_fd)
        close(dev_fd);
    munmap((void *)pNxECIDReg, mapsize);

    QString idstring;
    QTextStream in(&idstring);
    in.setFieldWidth(8);
    in.setPadChar('0');
    in.setIntegerBase(16);
    in << ECID[0] << ECID[1] << ECID[2] << ECID[3];
    result = true;
    QByteArray idbytes = idstring.toLatin1();
    //将id转为16字节
    if (result)
    {
        //加密
        unsigned char hash[16];
        unsigned char message[32];
        unsigned char key[32] = {
            0xED, 0x92, 0x02, 0xE6, 0x5F, 0x38, 0x30, 0x30, 0x55, 0xA2, 0x96, 0x2A, 0xE4, 0x03, 0x1F, 0x89,
            0x7A, 0x36, 0x32, 0x9E, 0x87, 0x0F, 0xF8, 0x63, 0x56, 0x38, 0x82, 0xA9, 0xB4, 0xB9, 0xB6, 0x53};

        ::memcpy(message, idbytes.data(), 32);

        monocypher::crypto_blake2b_general(hash, 16, key, 32, message, 32);
        /* Wipe secrets if they are no longer needed */
        monocypher::crypto_wipe(message, 32);
        monocypher::crypto_wipe(key, 32);
        idbytes.clear();
        idbytes.reserve(16);
        for (int nn = 0; nn != 16; nn++)
        {
            idbytes.append((char)hash[nn]);
        }
    }

    return idbytes;
#else
#error "Please Select Valid Platform!"
    //    //return QByteArray("123456789abcdefghijklmnopqrstuvw");
    //    unsigned int ECID[4]={0};
    //    int dev_fd;
    //    dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);
    //    if (dev_fd < 0)
    //    {
    //        KeyWarnOut::out("open(/dev/mem) failed.");
    //        result = false;
    //        return QByteArray(32,0xFF);
    //    }
    //    size_t mapsize = sizeof(struct NX_ECID_Reg);
    //    struct NX_ECID_Reg * pNxECIDReg = (struct NX_ECID_Reg *)mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, 0xC0067000 );
    //    ECID[0] = pNxECIDReg->ECID[0];
    //    ECID[1] = pNxECIDReg->ECID[1];
    //    ECID[2] = pNxECIDReg->ECID[2];
    //    ECID[3] = pNxECIDReg->ECID[3];

    //    if(dev_fd)
    //        close(dev_fd);
    //    munmap((void *)pNxECIDReg,mapsize);

    //    QString idstring;
    //    QTextStream in(&idstring);
    //    in.setFieldWidth(8);
    //    in.setPadChar('0');
    //    in.setIntegerBase(16);
    //    in<<ECID[0]<<ECID[1]<<ECID[2]<<ECID[3];
    //    result = true;
    //    return idstring.toLatin1();
    return QByteArray("ulitdbtest");
#endif
}

QByteArray KeyVerify::GetIDStringFromFile(const QString &path, bool &result)
{
    QFile idfile(path);
    QByteArray idbytes;
    idbytes.resize(16);
    idbytes.fill((char)0xFF);

    result = true;

    if (!idfile.open(QIODevice::ReadOnly))
    {
        //qDebug() << "Open ID File Failed:" << idfile.errorString();
        result = false;
    }
    else
    {
        QString chipidstr = QString(idfile.readAll()).trimmed();

        QByteArray chipidhex = hexStrToByteArray(chipidstr);
        idbytes.insert(0, chipidhex);
        idbytes.resize(16);
    }

    idfile.close();

    if (result)
    {
        //加密
        unsigned char hash[16];
        unsigned char message[16];
        unsigned char key[32] = {
            0xED, 0x92, 0x02, 0xE6, 0x3F, 0x38, 0x30, 0x30, 0x55, 0xA2, 0x96, 0x2A, 0xE4, 0x03, 0x1F, 0x89,
            0x7A, 0x36, 0x32, 0x9E, 0x87, 0x0F, 0xF8, 0x63, 0x56, 0x32, 0x82, 0xA9, 0xB4, 0xB9, 0xB6, 0x53};

        ::memcpy(message, idbytes.data(), 16);

        monocypher::crypto_blake2b_general(hash, 16, key, 32, message, 16);
        /* Wipe secrets if they are no longer needed */
        monocypher::crypto_wipe(message, 16);
        monocypher::crypto_wipe(key, 32);

        for (int nn = 0; nn != 16; nn++)
        {
            idbytes[nn] = hash[nn];
        }
    }

    return idbytes;
}

QByteArray KeyVerify::GetGanRaoBytes(int type, int num)
{
    QByteArray ganRaoArray;
    for (int i = 0; i < num; i++)
    {
        if (type == 0)
        {
            ganRaoArray.append((char)(qrand() % 10) + 0x30);
        }
        else if (type == 1)
        {
            if ((qrand() % 2))
            {
                ganRaoArray.append((char)(qrand() % 26) + 65);
            }
            else
            {
                ganRaoArray.append((char)(qrand() % 26) + 97);
            }
        }
        else
        {
            int subtype = qrand() % 3;
            if (subtype == 0)
                ganRaoArray.append((char)(qrand() % 10) + 0x30);
            else if (subtype == 1)
                ganRaoArray.append((char)(qrand() % 26) + 65);
            else
                ganRaoArray.append((char)(qrand() % 26) + 97);
        }
    }
    return ganRaoArray;
}

QByteArray KeyVerify::decryptRSAKeybyxchacha(const QString &keypath)
{
    QByteArray decrypt_key_array;

    decrypt_key_array.clear();

    QFile rsakeyfile(keypath);
    if (rsakeyfile.open(QIODevice::ReadOnly))
    {

        QByteArray b64bytes = rsakeyfile.readAll();

        QByteArray rsabytes = QByteArray::fromBase64(b64bytes);

        unsigned char *textbuf = new unsigned char[rsabytes.size()];
        memcpy(textbuf, rsabytes.data(), rsabytes.size());

        rsakeyfile.close();

        do {

            uint8_t key[32] = {
                0x08, 0x20, 0x7E, 0x9B, 0xD6, 0xA2, 0x86, 0x33, 0x4D, 0xB4, 0xA6, 0x97, 0x94, 0xB3, 0x52, 0x57,
                0xC1, 0xA9, 0xA7, 0xD9, 0x8E, 0x0A, 0x9C, 0x19, 0x61, 0xB4, 0xBE, 0x01, 0xE4, 0xE9, 0x96, 0xB1};

            uint8_t nonce[24] = {
                0x61, 0xB1, 0x51, 0xF5, 0x05, 0xE8, 0x92, 0x09, 0x56, 0x0E, 0xBB, 0x35,
                0x66, 0x41, 0x4F, 0xC8, 0x3A, 0xA3, 0x03, 0x26, 0xD9, 0x54, 0xB5, 0x27}; /* Use only once per key*/

            uint8_t *mac = (uint8_t *)(textbuf + rsabytes.size() - 16); /* Message authentication code */

            {

                if (monocypher::crypto_unlock(textbuf, key, nonce, mac, textbuf, rsabytes.size() - 16) != 0)
                {
                    /* The message is corrupted.
                     * Wipe key if it is no longer needed,
                     * and abort the decryption.
                     */
                    //qWarning("AES Key 解密失败.");
                    monocypher::crypto_wipe(key, 32);

                    return QByteArray();
                }
                else
                {

                    /* ...do something with the decrypted text here... */
                    decrypt_key_array = QByteArray((char *)textbuf, rsabytes.size() - 16);

                    /* Finally, wipe secrets if they are no longer needed */
                    monocypher::crypto_wipe(key, 32);
                }
            }

        } while (0);
    }
    else
    {
        //qWarning("打开AES Key 文件失败!");
        return QByteArray();
    }

    return decrypt_key_array;
}
