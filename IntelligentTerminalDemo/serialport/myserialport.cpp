#include "myserialport.h"

#include <QDebug>
#include <QThread>
#include <QSerialPortInfo>

MySerialPort::MySerialPort(bool connect, QObject *parent) : QObject(parent),
    serialPort(new QSerialPort())
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    isConnect = connect;
}

MySerialPort::~MySerialPort()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    closePort();
    delete serialPort;
}

bool MySerialPort::openPort(const QString &portName, int baudRate, QSerialPort::DataBits dataBits, QSerialPort::Parity parity, QSerialPort::StopBits stopBits, QSerialPort::FlowControl flowControl)
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setParity(parity);
    serialPort->setStopBits(stopBits);
    serialPort->setFlowControl(flowControl);

    bool isOk = serialPort->open(QIODevice::OpenModeFlag::ReadWrite);
    if (isOk && isConnect) {
        //QObject::connect(serialPort, SIGNAL(readyRead()),this, SLOT(readMsgSlot()));
        connect(serialPort, &QSerialPort::readyRead, this, &MySerialPort::readMsgSlot);
        connect(serialPort, &QSerialPort::errorOccurred, this, &MySerialPort::errorOccurredSlot);
    }
    return isOk;
}

void MySerialPort::closePort()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    if (serialPort && serialPort->isOpen()) {
        disconnect(serialPort, &QSerialPort::readyRead, this, &MySerialPort::readMsgSlot);
        disconnect(serialPort, &QSerialPort::errorOccurred, this, &MySerialPort::errorOccurredSlot);
        serialPort->close();
    }
}

bool MySerialPort::sendMessage(const QByteArray &message)
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    if (serialPort->isOpen()) {
        qint64 writeBytes = serialPort->write(message);
        qDebug() << "write length:" + QString::number(message.length());
        qDebug() << "write ret:" + QString::number(writeBytes);
        return serialPort->waitForBytesWritten();
    }
    return false;
}

QByteArray MySerialPort::readMessage()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    QByteArray data;
    if (serialPort->isOpen()) {
        qint64 num = serialPort->bytesAvailable();
        qDebug() << "bytesAvailable: " << num;
        if (num > 0) {
            data = serialPort->readAll();  // 读取所有可用数据
        }
    }
    return data;
}

QList<QString> MySerialPort::getAvaliablePorts()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    QList<QString> res;
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    foreach (QSerialPortInfo info, list) {
        qDebug() << "portName:" << info.portName();
        res.append(info.portName());
    }
    return res;
}

QByteArray MySerialPort::get_ic_test1() {
    //随机数据10字节
    unsigned char random0[10] = {0x93, 0x84, 0x01, 0x2F, 0xD7, 0x4F, 0x06, 0x51, 0xBD, 0x2A};

    //盐数据32字节
    unsigned char salt0[32] = {
        0x04, 0xE2, 0x12, 0xB7, 0x17, 0x72, 0xA2, 0xBF, 0x06, 0x84, 0xE0, 0x9D, 0xC7, 0xFF, 0xBF, 0x8C,
        0xB9, 0xA0, 0x28, 0x5E, 0x68, 0x98, 0x75, 0xFE, 0x47, 0x44, 0x80, 0x20, 0xD1, 0xFE, 0xE4, 0x26};

    //原始密码32字节
    unsigned char passwd[32] = {
        0xBA, 0x0B, 0x09, 0xBC, 0x1A, 0xD0, 0x00, 0xB5, 0x0C, 0xBE, 0xA7, 0x7A, 0x49, 0x8C, 0x24, 0xB8,
        0xF9, 0x2E, 0xD4, 0x10, 0x56, 0x63, 0x0B, 0x7E, 0x08, 0xE8, 0x5A, 0x9E, 0xDC, 0xCE, 0xD1, 0xFB};

    //制卡时间3字节
    unsigned char cardbuildtime[3] = {24, 7, 6};
    //到期时间3字节
    unsigned char cardendtime[3] = {24, 12, 1};
    //角色ID16字节
    unsigned char roleid[] = "1111222233334444";
    //原始卡号4字节
    unsigned char cardid[4] = {0xFD, 0xB5, 0x79, 0x9B};

    //加密数据

    //<1> 组合随机数据
    unsigned char raw_random_data[46];
    ::memcpy(raw_random_data, cardid, 4);
    ::memcpy(raw_random_data + 4, random0, 10);
    ::memcpy(raw_random_data + 4 + 10, salt0, 32);

    //<2> 扩展随机数据到64字节
    unsigned char H1[64];
    monocypher::crypto_blake2b(H1, raw_random_data, 46);
    monocypher::crypto_wipe(raw_random_data, 46);

    //<3> 获取nonc0和salt1
    unsigned char nonce0[24];
    unsigned char salt1[16];
    ::memcpy(nonce0, H1, 24);
    ::memcpy(salt1, H1 + 24, 16);

    //<4> 计算加密密码
    unsigned char Key1[32];
    const unsigned int nb_blocks = 12;
    const unsigned int nb_iterations = 3;
    void *work_area = malloc(nb_blocks * 1024);
    monocypher::crypto_argon2i(Key1, 32,
                               work_area, nb_blocks, nb_iterations,
                               (uint8_t *)passwd, 32,
                               salt1, 16);

    //<5> 构建用户数据
    unsigned char mac[16];
    unsigned char plaint_text[22];
    unsigned char cipher_text[22];
    ::memcpy(plaint_text, roleid, 16);
    ::memcpy(plaint_text + 16, cardbuildtime, 3);
    ::memcpy(plaint_text + 16 + 3, cardendtime, 3);

    //加密
    monocypher::crypto_lock(mac, cipher_text, Key1, nonce0, plaint_text, 22);

    //解密
    unsigned char plaint_text2[22];
    int ret = monocypher::crypto_unlock(plaint_text2, Key1, nonce0, mac, cipher_text, 22);

    if (ret == 0)
        qDebug() << "verify OK\n" << endl;
    else
        qDebug() << "Verify Failed\n" << endl;

    unsigned char icblock[16 * 3 + 5];
    icblock[0] = 0x01;
    ::memcpy(icblock + 1, cardid, 4);
    ::memcpy(icblock + 1 + 4, mac, 16);
    ::memcpy(icblock + 1 + 4 + 16, cipher_text, 22);
    ::memcpy(icblock + 1 + 4 + 16 + 22, random0, 10);
    encipherHelper.print_hex("source", icblock, sizeof(icblock));

    // 创建一个QByteArray来存储十六进制字符串
    QByteArray hexArray = EncipherTest::UcharToHex(icblock, sizeof(icblock));

    qDebug() << "hexArray:" << hexArray << endl;
    return hexArray;
}

void MySerialPort::cmdReadId()
{
    // 读卡号(卡序列号)：0xA1
    unsigned char readId[8];
    int kk = 0;
    readId[kk++] = 0x01;
    readId[kk++] = 0x08;
    readId[kk++] = 0xA1;
    readId[kk++] = 0x20;
    readId[kk++] = 0x00;
    readId[kk++] = 0x01;
    readId[kk++] = 0x00;
    readId[kk] = encipherHelper.calculate_checksum(readId, kk);
    int length = sizeof(readId);
    QByteArray hexArray = EncipherTest::UcharToHex(readId, length);
    sendMessage(hexArray);
}

void MySerialPort::cmdReadBlockHex()
{
    // 读指定块数据：0xA3
    unsigned char readBlock[8];
    int kk = 0;
    readBlock[kk++] = 0x01;
    readBlock[kk++] = 0x08;
    readBlock[kk++] = 0xA3;
    readBlock[kk++] = 0x20;
    readBlock[kk++] = 0x04;
    readBlock[kk++] = 0x01;
    readBlock[kk++] = 0x00;
    readBlock[kk] = encipherHelper.calculate_checksum(readBlock, kk);
    int length = sizeof(readBlock);
    QByteArray hexArray = EncipherTest::UcharToHex(readBlock, length);
    sendMessage(hexArray);
}

void MySerialPort::cmdWriteBlockHex()
{
    // 写数据到指定块：0xA4, 写16字节数据 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF 到块 4
    unsigned char writeBlock[23];
    int kk = 0;
    writeBlock[kk++] = 0x01;
    writeBlock[kk++] = 0x17;
    writeBlock[kk++] = 0xA4;
    writeBlock[kk++] = 0x20;
    writeBlock[kk++] = 0x04;
    writeBlock[kk++] = 0x01;
    writeBlock[kk++] = 0x00;
    writeBlock[kk++] = 0x11;
    writeBlock[kk++] = 0x22;
    writeBlock[kk++] = 0x33;
    writeBlock[kk++] = 0x44;
    writeBlock[kk++] = 0x55;
    writeBlock[kk++] = 0x66;
    writeBlock[kk++] = 0x77;
    writeBlock[kk++] = 0x88;
    writeBlock[kk++] = 0x99;
    writeBlock[kk++] = 0xAA;
    writeBlock[kk++] = 0xBB;
    writeBlock[kk++] = 0xCC;
    writeBlock[kk++] = 0xDD;
    writeBlock[kk++] = 0xEE;
    writeBlock[kk++] = 0xFF;
    writeBlock[kk] = encipherHelper.calculate_checksum(writeBlock, kk);

    int length = sizeof(writeBlock);
    writeBlock[length - 1] = encipherHelper.calculate_checksum(writeBlock, length - 1);
    QByteArray hexArray = EncipherTest::UcharToHex(writeBlock, length);
    sendMessage(hexArray);
}

void MySerialPort::cmdInitCard()
{
    // 初始化钱包：0xA6, 钱包的值范围：-2147483648 ~ 2147483647, 赋值 0 x12345678
    unsigned char initCard[11];
    int kk = 0;
    initCard[kk++] = 0x01;
    initCard[kk++] = 0x0B;
    initCard[kk++] = 0xA6;
    initCard[kk++] = 0x20;
    initCard[kk++] = 0x04;
    initCard[kk++] = 0x00;
    initCard[kk++] = 0x78;
    initCard[kk++] = 0x56;
    initCard[kk++] = 0x34;
    initCard[kk++] = 0x12;
    initCard[kk] = encipherHelper.calculate_checksum(initCard, kk);

    int length = sizeof(initCard);
    initCard[length - 1] = encipherHelper.calculate_checksum(initCard, length - 1);
    QByteArray hexArray = EncipherTest::UcharToHex(initCard, length);
    sendMessage(hexArray);
}

void MySerialPort::cmdReadCard()
{
    // 钱包值（余额）查询：0xA9
    unsigned char readCard[8];
    int kk = 0;
    readCard[kk++] = 0x01;
    readCard[kk++] = 0x08;
    readCard[kk++] = 0xA9;
    readCard[kk++] = 0x20;
    readCard[kk++] = 0x04;
    readCard[kk++] = 0x00;
    readCard[kk++] = 0x00;
    readCard[kk] = encipherHelper.calculate_checksum(readCard, kk);

    int length = sizeof(readCard);
    readCard[length - 1] = encipherHelper.calculate_checksum(readCard, length - 1);
    QByteArray hexArray = EncipherTest::UcharToHex(readCard, length);
    sendMessage(hexArray);
}

void MySerialPort::cmdCardAdd()
{
    // 钱包充值（加值）：0xA8, 余额（原值）100, 值加 58
    unsigned char cardAdd[11];
    int kk = 0;
    cardAdd[kk++] = 0x01;
    cardAdd[kk++] = 0x0B;
    cardAdd[kk++] = 0xA8;
    cardAdd[kk++] = 0x20;
    cardAdd[kk++] = 0x04;
    cardAdd[kk++] = 0x01;
    cardAdd[kk++] = 0x3A;
    cardAdd[kk++] = 0x00;
    cardAdd[kk++] = 0x00;
    cardAdd[kk++] = 0x00;
    cardAdd[kk] = encipherHelper.calculate_checksum(cardAdd, kk);

    int length = sizeof(cardAdd);
    cardAdd[length - 1] = encipherHelper.calculate_checksum(cardAdd, length - 1);
    QByteArray hexArray = EncipherTest::UcharToHex(cardAdd, length);
    sendMessage(hexArray);
}

void MySerialPort::cmdCardDel()
{
    // 钱包扣款（减值）：0xA7, 余额（原值）100, 值减 58
    unsigned char cardDel[11];
    int kk = 0;
    cardDel[kk++] = 0x01;
    cardDel[kk++] = 0x0B;
    cardDel[kk++] = 0xA7;
    cardDel[kk++] = 0x20;
    cardDel[kk++] = 0x04;
    cardDel[kk++] = 0x01;
    cardDel[kk++] = 0x3A;
    cardDel[kk++] = 0x00;
    cardDel[kk++] = 0x00;
    cardDel[kk++] = 0x00;
    cardDel[kk] = encipherHelper.calculate_checksum(cardDel, kk);

    int length = sizeof(cardDel);
    cardDel[length - 1] = encipherHelper.calculate_checksum(cardDel, length - 1);
    QByteArray hexArray = EncipherTest::UcharToHex(cardDel, length);
    sendMessage(hexArray);
}

bool MySerialPort::isOpen()
{
    return serialPort && serialPort->isOpen();
}

short MySerialPort::testValue() const
{
    return mTestValue;
}

void MySerialPort::setTestValue(short newTestValue)
{
    if (mTestValue == newTestValue)
        return;
    mTestValue = newTestValue;
    emit testValueChanged();
}

bool MySerialPort::send_ic_test()
{
    QByteArray hexArray = get_ic_test1();

    if (serialPort->isOpen()) {
        qint64 writeBytes = serialPort->write(hexArray);
        qDebug() << "write ret:" + QString::number(writeBytes);
        return serialPort->waitForBytesWritten();
    }
    return false;
}

void MySerialPort::readMsgSlot()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    while (serialPort->bytesAvailable()) {
        qint64 readBytes = serialPort->bytesAvailable();
        qDebug() << "read ret:" + QString::number(readBytes);
        QByteArray message = serialPort->readAll();  // 读取所有可用数据
        qDebug() << "read length:" + QString::number(message.length());

        emit readMsgSignal(message);  // 发送信号
    }
}

void MySerialPort::icDataVerifyTest(const QByteArray &icbytes)
{
    //读卡器ID(1字节)+读卡器端口IC卡号(4bytes)+块号4数据(16bytes)+块号5数据(16bytes)+块号6数据(16bytes)
    //  块4: 消息认证码(16字节)
    //  块5: 加密数据(16字节)
    //  块6: 剩余加密数据(6字节) + 随机数据random0(10字节)

    //用户数据=角色ID(16字节)+制卡时间(3字节,年月日)+到期时间(3字节,年月日)

    bool result = false;

    qDebug() << "icbytes:" << icbytes << endl;

    auto icdata = EncipherTest::HexToUchar(icbytes);
    //const unsigned char *icdata = (const unsigned char *)icbytes.data();
    encipherHelper.print_hex("target", icdata.get(), icbytes.length() / 2);

    //随机数据10字节
    const unsigned char *random0 = &icdata[1 + 4 + 16 + 16 + 6];

    //盐数据32字节
    unsigned char salt0[32] = {
        0x04, 0xE2, 0x12, 0xB7, 0x17, 0x72, 0xA2, 0xBF, 0x06, 0x84, 0xE0, 0x9D, 0xC7, 0xFF, 0xBF, 0x8C,
        0xB9, 0xA0, 0x28, 0x5E, 0x68, 0x98, 0x75, 0xFE, 0x47, 0x44, 0x80, 0x20, 0xD1, 0xFE, 0xE4, 0x26};

    //原始密码32字节
    unsigned char passwd[32] = {
        0xBA, 0x0B, 0x09, 0xBC, 0x1A, 0xD0, 0x00, 0xB5, 0x0C, 0xBE, 0xA7, 0x7A, 0x49, 0x8C, 0x24, 0xB8,
        0xF9, 0x2E, 0xD4, 0x10, 0x56, 0x63, 0x0B, 0x7E, 0x08, 0xE8, 0x5A, 0x9E, 0xDC, 0xCE, 0xD1, 0xFB};

    //原始卡号4字节
    const unsigned char *cardid = &icdata[1];

    QString out_card_id;
    QString out_role_id;
    QDate out_card_build_time;
    QDate out_card_end_time;

    do {

        //<1> 组合随机数据
        unsigned char raw_random_data[46];
        ::memcpy(raw_random_data, cardid, 4);
        ::memcpy(raw_random_data + 4, random0, 10);
        ::memcpy(raw_random_data + 4 + 10, salt0, 32);

        //<2> 扩展随机数据到64字节
        unsigned char H1[64];
        monocypher::crypto_blake2b(H1, raw_random_data, 46);
        monocypher::crypto_wipe(raw_random_data, 46);

        //<3> 获取nonc0和salt1
        unsigned char nonce0[24];
        unsigned char salt1[16];
        ::memcpy(nonce0, H1, 24);
        ::memcpy(salt1, H1 + 24, 16);

        //<4> 计算加密密码
        unsigned char Key1[32];
        const unsigned int nb_blocks = 12;
        const unsigned int nb_iterations = 3;
        void *work_area = malloc(nb_blocks * 1024);
        monocypher::crypto_argon2i(Key1, 32,
                                   work_area, nb_blocks, nb_iterations,
                                   (uint8_t *)passwd, 32,
                                   salt1, 16);

        //<5> 解密
        const unsigned char *mac = &icdata[5];
        unsigned char plaint_text[22];
        int ret = monocypher::crypto_unlock(plaint_text, Key1, nonce0, mac, &icdata[5 + 16], 22);
        if (ret != 0)
        {
            //验证失败
            break;
        }

        //<6> 计算卡号
        unsigned char cardid_real[16 + 4];
        ::memcpy(cardid_real, cardid, 4);
        monocypher::crypto_blake2b_general(&cardid_real[4], 16, 0, 0, mac, 16);
        out_card_id = QByteArray((const char *)&cardid_real[0], 20).toHex().toUpper();

        //<7> 解析数据
        //用户角色id
        out_role_id = QString::fromLocal8Bit((const char *)&plaint_text[0], 16);

        //制卡时间
        out_card_build_time = QDate(2000 + plaint_text[16], plaint_text[17], plaint_text[18]);

        //到期时间
        out_card_end_time = QDate(2000 + plaint_text[19], plaint_text[20], plaint_text[21]);

        result = true;

    } while (0);

    monocypher::crypto_wipe(passwd, 32);
    monocypher::crypto_wipe(salt0, 32);

    qDebug() << "验证:" << result << endl;
    if (result) {
        qDebug() << "卡号:" << out_card_id << endl;
        qDebug() << "用户角色id:" << out_role_id << endl;
        qDebug() << "制卡时间:" << out_card_build_time << endl;
        qDebug() << "到期时间:" << out_card_end_time << endl;
    }
}

void MySerialPort::errorOccurredSlot()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    qDebug() << serialPort->errorString();
}


