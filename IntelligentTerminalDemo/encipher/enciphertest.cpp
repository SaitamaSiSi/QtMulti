#include "enciphertest.h"

EncipherTest::EncipherTest(QObject *parent) : QObject(parent)
{

}

void EncipherTest::monocypherTest()
{
    //数据内存分布
    //
    //  块4: 消息认证码(16字节)
    //  块5: 加密数据(16字节)
    //  块6: 剩余加密数据(6字节) + 随机数据random0(10字节)
    //

    // 已知数据: random0, salt0, passwd
    // 用户数据=角色ID(16字节)+制卡时间(3字节,年月日)+到期时间(3字节,年月日)
    // 加密数据,消息认证码=xchacha20-poly1305(用户数据, Key1, Nonce0)
    // 实际卡号=原始卡号(4字节) + Blake2b(消息认证码,no_key)生成Hash(16字节)， 卡号使用16进制字符串表示

    // xchacha20-poly1305算法key和nonc获取
    // <1> 构建数据D1=原始卡号(4字节)+随机数据random0(10字节)+盐数据salt0(32字节)
    // <2> 计算hash值H1(64字节)=Blacke2b(数据D1)
    // <3> 取nonce0=H1[0,23]  salt1=H1[24,39]
    // <4> 使用Argon2i算法计算加密Key1=Argon2i(passwd, salt1, no_key, no_ad, nb_blocks=12, nb_iterations=3)
    // <5> 加密数据,消息认证码=xchacha20-poly1305(用户数据, Key1, nonce0)

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
        printf("verify OK\n");
    else
        printf("Verify Failed\n");

    //<6> 计算卡号
    unsigned char cardid_real[16 + 4];
    ::memcpy(cardid_real, cardid, 4);
    monocypher::crypto_blake2b_general(&cardid_real[4], 16, 0, 0, mac, 16);

    print_hex("raw_cardid    :", cardid, 4);
    print_hex("role_id       :", roleid, 16);
    print_hex("card_buildtime:", cardbuildtime, 3);
    print_hex("card_endtime  :", cardendtime, 3);
    print_hex("raw_cfg_data  :", plaint_text, 22);
    print_hex("random0       :", random0, 10);
    print_hex("salt0         :", salt0, 32);
    print_hex("passwd        :", random0, 32);

    printf("\n------------------------------------------\n");
    print_hex("mac     :", mac, 16);
    print_hex("cipher  :", cipher_text, 22);
    print_hex("cardid  :", cardid_real, 20);
    printf("\n------------------------------------------\n");
    //IC卡内部数据格式
    printf("IC Card Memory:\n");
    print_hex("[4] ", mac, 16);
    print_hex("[5] ", cipher_text, 16);
    print_hex("[6] ", &cipher_text[16], 6, 1);
    print_hex("", random0, 10);

    //  打印结果:
    //    raw_cardid    :: FD B5 79 9B
    //    role_id       :: 31 31 31 31 32 32 32 32 33 33 33 33 34 34 34 34
    //    card_buildtime:: 18 07 06
    //    card_endtime  :: 18 0C 01
    //    raw_cfg_data  :: 31 31 31 31 32 32 32 32 33 33 33 33 34 34 34 34 18 07 06 18 0C 01
    //    random0       :: 93 84 01 2F D7 4F 06 51 BD 2A
    //    salt0         :: 04 E2 12 B7 17 72 A2 BF 06 84 E0 9D C7 FF BF 8C B9 A0 28 5E 68 98 75 FE 47 44 80 20 D1 FE E4 26
    //    passwd        :: 93 84 01 2F D7 4F 06 51 BD 2A 00 00 00 00 00 00 EC 9D 77 8E 1E F4 1A 41 BF BC 58 61 A2 B3 8E E6

    //    ------------------------------------------
    //    mac     :: E5 E5 1A 45 61 49 B4 71 41 E4 EB 0F 4F D7 03 F8
    //    cipher  :: 0D DE 18 73 9B C2 7F 42 75 B0 AF AA A3 4E 4B F7 AD 08 7C 40 0F DD
    //    cardid  :: FD B5 79 9B D0 F5 6B 80 54 B0 95 6B 32 85 62 D9 1F 8F 62 AB

    //    ------------------------------------------
    //    IC Card Memory:
    //    [4] : E5 E5 1A 45 61 49 B4 71 41 E4 EB 0F 4F D7 03 F8
    //    [5] : 0D DE 18 73 9B C2 7F 42 75 B0 AF AA A3 4E 4B F7
    //    [6] : AD 08 7C 40 0F DD 93 84 01 2F D7 4F 06 51 BD 2A
    //    1: : FD F1 00 15 03 15 C3 20 11 22 33 44 55 66 78 77 88 69 66 55 44 33 22 11 E4
    //    1: : FD F1 00 17 01 17 5B 20 04 01 E5 E5 1A 45 61 49 B4 71 41 E4 EB 0F 4F D7 03 F8 07
    //    1: : FD F1 00 17 01 17 5B 20 05 01 0D DE 18 73 9B C2 7F 42 75 B0 AF AA A3 4E 4B F7 DB
    //    1: : FD F1 00 17 01 17 5B 20 06 01 AD 08 7C 40 0F DD 93 84 01 2F D7 4F 06 51 BD 2A BF

    unsigned char icblock[16 * 3];
    ::memcpy(icblock, mac, 16);
    ::memcpy(icblock + 16, cipher_text, 22);
    ::memcpy(icblock + 16 + 22, random0, 10);

    build_ext_cmd(1, build_update_keyA_cmd());
    build_ext_cmd(1, build_ic_write_block_cmd(4, &icblock[0]));
    build_ext_cmd(1, build_ic_write_block_cmd(5, &icblock[16]));
    build_ext_cmd(1, build_ic_write_block_cmd(6, &icblock[32]));

    monocypher::crypto_wipe(passwd, 32);
    monocypher::crypto_wipe(random0, 10);
    monocypher::crypto_wipe(salt0, 32);
}

void EncipherTest::getCharRandom(unsigned char *buf, int buflen, unsigned char minvalue, unsigned char maxvalue)
{
    int nn = 0;
    static std::random_device rd;
    static std::mt19937 e(rd());
    static std::uniform_int_distribution<unsigned char> gen(minvalue, maxvalue);

    for (nn = 0; nn < buflen; nn++) buf[nn] = gen(e);
}

void EncipherTest::print_hex(const char *flag, const unsigned char *buf, int len, int no_line_break)
{
    if (strlen(flag))
        printf("%s: ", flag);

    for (int ii = 0; ii < len; ii++)
    {
        printf("%02X ", buf[ii]);
        //        if (((ii + 1) % 16 == 0) && ((ii + 1) != len)) printf("\n");
    }
    if (!no_line_break) printf("\n");
}

unsigned char EncipherTest::calculate_checksum(const unsigned char *buf, int len)
{
    unsigned char checksum = 0;
    for (int ii = 0; ii < len; ii++) checksum ^= buf[ii];
    return ~checksum;
}

QByteArray EncipherTest::build_ic_write_block_cmd(unsigned char block_num, const unsigned char *data)
{
    unsigned char sendbuf[32];
    unsigned int kk = 0;

    sendbuf[kk++] = 0x01;
    sendbuf[kk++] = 0x17;
    sendbuf[kk++] = 0x5B;
    sendbuf[kk++] = 0x20;
    sendbuf[kk++] = block_num;  //块号
    sendbuf[kk++] = 0x01;
    for (int ii = 0; ii < 16; ii++) sendbuf[kk++] = data[ii];

    sendbuf[kk] = calculate_checksum(sendbuf, kk);

    QByteArray out((char *)sendbuf, kk + 1);
    return out;
}

QByteArray EncipherTest::build_update_keyA_cmd()
{
    //设备端仅需要keyA用与读取块数据, 旧密码和keyB随意填写, 控制位: 78778869
    unsigned char sendbuf[32];
    unsigned int kk = 0;

    unsigned char keyA[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    unsigned char keyB[6] = {0x66, 0x55, 0x44, 0x33, 0x22, 0x11};

    sendbuf[kk++] = 0x03;
    sendbuf[kk++] = 0x15;
    sendbuf[kk++] = 0xC3;
    sendbuf[kk++] = 0x20;
    for (int ii = 0; ii < 6; ii++) sendbuf[kk++] = keyA[ii];  //keyA
    sendbuf[kk++] = 0x78;                                     //control
    sendbuf[kk++] = 0x77;
    sendbuf[kk++] = 0x88;
    sendbuf[kk++] = 0x69;
    for (int ii = 0; ii < 6; ii++) sendbuf[kk++] = keyB[ii];

    sendbuf[kk] = calculate_checksum(sendbuf, kk);

    QByteArray out((char *)sendbuf, kk + 1);
    return out;
}

QByteArray EncipherTest::build_ext_cmd(int portnum, const QByteArray &data)
{
    QByteArray m_sendbytes;
    int kk = 0;
    int datasize = data.size();
    int sndbytes = 4 + datasize;
    m_sendbytes.resize(sndbytes);
    unsigned char *tmpbuf = (unsigned char *)m_sendbytes.data();

    tmpbuf[kk++] = 0xFD;
    tmpbuf[kk++] = portnum + 0xF0;
    tmpbuf[kk++] = (datasize >> 8) & 0xFF;
    tmpbuf[kk++] = datasize & 0xFF;
    ::memcpy(&tmpbuf[kk], data.data(), datasize);

    print_hex("1: ", tmpbuf, sndbytes);

    QByteArray res((const char*)tmpbuf, sndbytes);

    return res;
}

void EncipherTest::rsaTest()
{
    std::string pKeyStr = "./pKey";
    std::string sKeyStr = "./sKey";
    const char* pKey;
    const char* sKey;
    size_t pLength = strlen(pKeyStr.data());
    size_t sLength = strlen(sKeyStr.data());
    char* dynamicPKeyString = (char*)malloc(pLength + 1); // 分配足够的内存来存储字符串和终止的null字符
    char* dynamicSKeyString = (char*)malloc(sLength + 1); // 分配足够的内存来存储字符串和终止的null字符
    if (dynamicPKeyString) {
        strcpy(dynamicPKeyString, pKeyStr.data()); // 复制字符串
        pKey = dynamicPKeyString; // 初始化const char*指针
    }
    if (dynamicSKeyString) {
        strcpy(dynamicSKeyString, sKeyStr.data()); // 复制字符串
        sKey = dynamicSKeyString; // 初始化const char*指针
    }

    RSAHelper::generateRSAKey(pKey, sKey, 3072);

    free(dynamicPKeyString); // 释放动态分配的内存
    free(dynamicSKeyString); // 释放动态分配的内存



    QString content = "cecesh错误信iaW2213";
    qDebug() << "content:" << content;
    QByteArray sourceByteArray = content.toUtf8();
    qDebug() << "sourceByteArray:" << sourceByteArray;

    /* Encode */
    QByteArray pKeyArray;
    QFile rsapkeyfile(QString::fromStdString(pKeyStr));
    if (rsapkeyfile.open(QIODevice::ReadOnly))
    {
        pKeyArray = rsapkeyfile.readAll();
    }
    rsapkeyfile.close();
    int encryptlen = RSAHelper::calcDataEncryptLegth(sourceByteArray.size(), 3072);
    int finallen = 0;
    QByteArray finalkeycontext;
    char *tmpclipher = new char[encryptlen];
    finallen = RSAHelper::pubEncrypt(tmpclipher, sourceByteArray.data(), sourceByteArray.size(), pKeyArray.data(), pKeyArray.size());
    if (finallen > 0)
    {
        finalkeycontext = Base64Helper::encode(QByteArray(tmpclipher, finallen));  //QByteArray(tmpclipher, finallen).toBase64();
    }
    delete[] tmpclipher;
    qDebug() << "finalkeycontext:" << finalkeycontext;

    /* Decode */
    QByteArray sKeyArray;
    QFile rsaskeyfile(QString::fromStdString(sKeyStr));
    if (rsaskeyfile.open(QIODevice::ReadOnly))
    {
        sKeyArray = rsaskeyfile.readAll();
    }
    rsaskeyfile.close();
    QByteArray cliphercontext = Base64Helper::decode(finalkeycontext);  //QByteArray::fromBase64(encrypData);
    char *tmpplain = new char[cliphercontext.size()];
    int plainlen = RSAHelper::priDecrypt(tmpplain, cliphercontext.data(), cliphercontext.size(), sKeyArray.data(), sKeyArray.size());
    QByteArray plaincontext(tmpplain, plainlen);
    delete[] tmpplain;
    qDebug() << "decodeByteArray:" << plaincontext;

    QString target = QString::fromUtf8(plaincontext);
    qDebug() << "target:" << target << endl;
}

void EncipherTest::Base64Test()
{
    QString content = "cecesh错误信iaW2213";
    qDebug() << "content:" << content;
    QByteArray sourceByteArray = content.toUtf8();
    qDebug() << "sourceByteArray:" << sourceByteArray;
    QByteArray encodeByteArray = Base64Helper::encode(sourceByteArray);
    qDebug() << "encodeByteArray:" << encodeByteArray;
    QByteArray decodeByteArray = Base64Helper::decode(encodeByteArray);
    qDebug() << "decodeByteArray:" << decodeByteArray;
    QString target = QString::fromUtf8(decodeByteArray);
    qDebug() << "target:" << target << endl;
}

int EncipherTest::getDigitValue(QChar ch)
{
    if (ch.isDigit()) {
        return ch.digitValue();
    } else {
        return ch.toLatin1() - 'A' + 10;
    }
}

QByteArray EncipherTest::UcharToHex(unsigned char * block, unsigned long long len)
{
    // 创建一个QByteArray来存储十六进制字符串
    QByteArray hexArray;
    // 遍历数组，并转换为十六进制字符串
    for ( unsigned long long i = 0; i < len; ++i) {
        // 将unsigned char转换为十六进制字符串，并移除前导零
        QString hexStr = QString("%1").arg(static_cast<int>(block[i]), 2, 16, QChar('0'));
        hexArray.append(hexStr);
    }
    return hexArray;
}

std::unique_ptr<unsigned char[]> EncipherTest::HexToUchar(const QByteArray &hexArray) {
    std::unique_ptr<unsigned char[]> data(new unsigned char[hexArray.length() / 2]);
    // 转换QByteArray到unsigned char数组
    for (int i = 0; i < hexArray.size(); i += 2) { // 每个十六进制数是两个字符
        // 提取两个字符
        QChar high = hexArray.at(i);
        QChar low = hexArray.at(i + 1);

        // 将两个QChar转换为十六进制整数
        int highValue = getDigitValue(high.toUpper());
        int lowValue = getDigitValue(low.toUpper());

        // 组合高低位到unsigned char
        data[i / 2] = (highValue << 4) | lowValue;
    }
    return data; // 返回std::unique_ptr
}

