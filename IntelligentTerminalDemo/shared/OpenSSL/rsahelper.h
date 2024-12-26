#ifndef RSAHELPER_H
#define RSAHELPER_H

#include <string>

class RSAHelper
{
public:
    RSAHelper();

    //!生成公钥和私钥
    //! key_bits: 密钥位数
    static void generateRSAKey(const char *pub_key_path ,const char *pri_key_path, int key_bits=3072);

    //!计算加密后数据长度
    //! text_len: 明文长度
    //! key_len:加密密钥长度
    static int calcDataEncryptLegth(int text_len, int key_bits);

    //!使用公钥加密数据
    //! clipher_text:  存储加密后数据缓冲,该缓冲必须预先分配，加密后数据长度可以使用calcDataEncryptLegth计算
    //! plain_text  :  明文数据
    //! text_len    :  明文长度
    //! pub_key     :  存储公钥缓冲地址
    //! key_len     :  公钥缓冲大小
    //!返回: 加密后数据长度 0=失败
    static int pubEncrypt(char *cipher_text, const char *plain_text, int plain_len, const char *pub_key, int key_size);

    //!使用私钥解密
    //! plain_text  : 存储解密后明文数据缓冲，该缓冲必须预先分配，缓冲区长度建议为密文长度
    //! cipher_text : 加密数据
    //! cipher_len  : 密文长度
    //! pri_key     : 存储私钥缓冲地址
    //! key_len     : 私钥缓冲大小
    //! 返回: 解密后明文实际长度
    static int priDecrypt(char *plain_text, const char *cipher_text, int cipher_len, const char *pri_key, int key_size);
};

#endif // RSAHELPER_H
