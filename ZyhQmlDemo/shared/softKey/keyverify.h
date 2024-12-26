#ifndef KEYVERIFY_H
#define KEYVERIFY_H

//#include <cryptopp/base64.h>
//#include <cryptopp/files.h>
//#include <cryptopp/hex.h>
//#include <cryptopp/misc.h>
//#include <cryptopp/osrng.h>
//#include <cryptopp/rsa.h>

#include <QObject>
#include <QString>

//USING_NAMESPACE(CryptoPP)

class KeyVerify : public QObject
{
    Q_OBJECT
public:
    explicit KeyVerify(QObject *parent = 0);

    static QByteArray GetIDString2(bool &result);
    static QByteArray GetIDStringFromFile(const QString &path, bool &result);
signals:

public slots:
    bool Encryp(const QString &keyOutPath, const QString &cpuidpath = QString());
    bool Verify(const QString &keyPath, const QString &cpuidpath = QString());

private:
    bool Encryp_(QByteArray &origdata, const QString &keyOutPath);
    QByteArray EncrypPriv(const QByteArray &Origdata);
    QByteArray DecrypPriv(const QByteArray &encrypData);
    QByteArray GetGanRaoBytes(int type, int num);
    QByteArray decryptRSAKeybyxchacha(const QString &keypath);
    static QByteArray GetIDString(bool &result);
};

#endif  // KEYVERIFY_H
