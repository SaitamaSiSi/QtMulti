#include "base64helper.h"

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// base64 编码
QByteArray Base64Helper::encode(const QByteArray &input)
{
#if 0
#if 0
    int maxencodelen = 4 * ((input.size() + 2) / 3);
    unsigned char *output = new unsigned char[maxencodelen];  //+1 for the terminating null that EVP_EncodeBlock adds on

    int encodelen = EVP_EncodeBlock(output, (const unsigned char *)input.data(), input.length());

    QByteArray b64bytes((const char *)output, encodelen);

    delete[] output;

    return b64bytes;
#else
    BIO *sink = NULL;
    BIO *b64 = NULL;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    //BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    sink = BIO_new(BIO_s_mem());
    BIO_push(b64, sink);
    BIO_write(b64, (const unsigned char *)input.data(), input.size());
    BIO_flush(b64);

    BIO_get_mem_ptr(b64, &bptr);
    BIO_set_close(b64, BIO_NOCLOSE);

    QByteArray b64bytes(bptr->data, bptr->length);

    BIO_free_all(b64);

    return b64bytes;
#endif
#else
    return input.toBase64();
#endif
}

// base64 解码
QByteArray Base64Helper::decode(const QByteArray &input)
{
#if 0
#if 0
    const auto maxdecodelen = 3 * input.size() / 4;
    unsigned char *output = new unsigned char[maxdecodelen];
    int decodelen = EVP_DecodeBlock(output, (const unsigned char *)input.data(), input.length());

    QByteArray b64bytes((const char *)output, decodelen);

    delete[] output;

    return b64bytes;
#else

    BIO *b64 = BIO_new(BIO_f_base64());
    //BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO *source = BIO_new_mem_buf(input.data(), input.length());  // read-only source
    BIO_push(b64, source);
    const int maxlen = input.length() / 4 * 3 + 1;
    QByteArray decoded;
    decoded.resize(maxlen);
    const int len = BIO_read(b64, decoded.data(), maxlen);
    decoded.resize(len);

    BIO_set_close(b64, BIO_NOCLOSE);
    BIO_free_all(b64);

    return decoded;
#endif
#else
    return QByteArray::fromBase64(input);
#endif
}
