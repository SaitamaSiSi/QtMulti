#ifndef BASE64HELPER_H
#define BASE64HELPER_H

#include <QByteArray>

class Base64Helper
{
public:
    //!Base64编码
    static QByteArray encode(const QByteArray &input);
    // base64 解码
    static QByteArray decode(const QByteArray &input);
};

#endif  // BASE64HELPER_H
