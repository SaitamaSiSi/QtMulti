#ifndef ENCIPHERTEST_H
#define ENCIPHERTEST_H

#include <QObject>
#include <QDebug>
#include "base64helper.h"
#include "rsahelper.h"
#include "helptools.h"

namespace monocypher {
//    extern "C" {
#include "monocypher.h"
//    }
}  // namespace monocypher


class EncipherTest : public QObject
{
    Q_OBJECT
public:
    explicit EncipherTest(QObject *parent = nullptr);
    /* monocypherTest */
    void monocypherTest();
    void getCharRandom(unsigned char *buf, int buflen, unsigned char minvalue, unsigned char maxvalue);
    unsigned char calculate_checksum(const unsigned char *buf, int len);
    QByteArray build_ic_write_block_cmd(unsigned char block_num, const unsigned char *data);
    QByteArray build_update_keyA_cmd();
    //1.主串口下发COM2: FD 通道1byte 数据长度N(2byte,高在前) 数据Nbyte
    //  通道: F1..F6:表示目的串口1..6
    QByteArray build_ext_cmd(int portnum, const QByteArray &data);

    void rsaTest();

    void Base64Test();

    static void print_hex(const char *flag, const unsigned char *buf, int len, int no_line_break = 0);
    static int getDigitValue(QChar);
    static QByteArray UcharToHex(unsigned char*,  unsigned long long);
    static std::unique_ptr<unsigned char[]> HexToUchar(const QByteArray &);

private:

signals:

};

#endif // ENCIPHERTEST_H
