#include "rsahelper.h"

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#ifndef __ARM_EABI__
// 关闭 RSA_generate_key/PEM_write_bio_RSAPrivateKey/RSA_free/PEM_read_bio_RSA_PUBKEY接口产生的警告
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

extern "C" {
#include <stdio.h>
#include <string.h>
}

RSAHelper::RSAHelper()
{
}

void RSAHelper::generateRSAKey(const char *pub_key_path, const char *pri_key_path, int key_bits)
{
    size_t pri_len = 0;       // 私钥长度
    size_t pub_len = 0;       // 公钥长度
    char *pri_key = nullptr;  // 私钥
    char *pub_key = nullptr;  // 公钥
    FILE *pub_file = NULL;
    FILE *pri_file = NULL;

    // 生成密钥对
    RSA *keypair = RSA_generate_key(key_bits, RSA_F4, NULL, NULL);

    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());

    // 生成私钥
    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    // 注意------生成第1种格式的公钥
    //PEM_write_bio_RSAPublicKey(pub, keypair);
    // 注意------生成第2种格式的公钥（此处代码中使用这种）
    PEM_write_bio_RSA_PUBKEY(pub, keypair);

    // 获取长度
    pri_len = BIO_pending(pri);
    pub_len = BIO_pending(pub);

    // 密钥对读取到字符串
    pri_key = (char *)malloc(pri_len + 1);
    pub_key = (char *)malloc(pub_len + 1);

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    // 将公钥写入文件
    pub_file = fopen(pub_key_path, "w+");
    if (pub_file == NULL)
    {
        perror("pub key file open fail:");
        return;
    }
    fwrite(pub_key, sizeof(char), pub_len, pub_file);
    fclose(pub_file);

    // 将私钥写入文件
    pri_file = fopen(pri_key_path, "w+");
    if (pri_file == NULL)
    {
        perror("pri key file open fail:");
        return;
    }
    fwrite(pri_key, sizeof(char), pri_len, pri_file);
    fclose(pri_file);

    // 释放内存
    RSA_free(keypair);
    BIO_free_all(pub);
    BIO_free_all(pri);

    free(pri_key);
    free(pub_key);
}

int RSAHelper::pubEncrypt(char *cipher_text, const char *plain_text, int plain_len, const char *pub_key, int key_size)
{
    int ret = 0;
    //int clipher_pos = 0;
    int clipher_len = 0;
    int plain_pos = 0;
    int plain_block_len = 0;
    int plain_last_len = plain_len;

    char *temp_cipher_text = cipher_text;
    if (cipher_text == NULL || plain_text == NULL) return 0;

    BIO *keybio = BIO_new_mem_buf(pub_key, key_size);
    RSA *rsa = RSA_new();
    // 注意-----第1种格式的公钥
    //rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
    // 注意-----第2种格式的公钥（这里以第二种格式为例）
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    if (rsa == NULL)
    {
        unsigned long err = ERR_get_error();  //获取错误号
        char err_msg[1024] = {0};
        ERR_error_string(err, err_msg);  // 格式：error:errId:库:函数:原因
        printf("err msg: err:%ld, msg:%s\n", err, err_msg);

        BIO_free_all(keybio);
        RSA_free(rsa);

        return 0;
    }

    // 获取RSA单次可以处理的数据块的最大长度
    int unit_data_len = RSA_size(rsa);
    // flen must be less than RSA_size(rsa) - 11 for the PKCS #1 v1.5 based
    // padding modes, less than RSA_size(rsa) - 41 for RSA_PKCS1_OAEP_PADDING
    // and exactly RSA_size(rsa) for RSA_NO_PADDING.
    int block_len = unit_data_len - 41;  // 填充方式为RSA_PKCS1_PADDING, 要在key_len基础上减去11
                                         // 填充方式为RSA_PKCS1_OAEP_PADDING, 要在key_len基础上减去41

    // 对数据进行分段加密（返回值是加密后数据的长度）
    while (plain_pos < plain_len)
    {

        plain_block_len = (plain_last_len > block_len) ? block_len : plain_last_len;

        //RSA_public_encrypt(int flen, unsigned char *from,unsigned char *to, RSA *rsa, int padding);
        ret = RSA_public_encrypt(plain_block_len, (const unsigned char *)(plain_text + plain_pos), (unsigned char *)temp_cipher_text, rsa, RSA_PKCS1_OAEP_PADDING);
        if (ret < 0)
        {
            clipher_len = 0;
            break;
        }

        temp_cipher_text += ret;
        clipher_len += ret;
        plain_pos += plain_block_len;
        plain_last_len -= plain_block_len;
    }

    // 释放内存
    BIO_free_all(keybio);
    RSA_free(rsa);

    return clipher_len;
}

int RSAHelper::priDecrypt(char *plain_text, const char *cipher_text, int cipher_len, const char *pri_key, int key_size)
{
    int cipher_pos = 0;
    int plain_pos = 0;
    int ret = 0;

    if (cipher_text == NULL || plain_text == NULL) return 0;

    RSA *rsa = RSA_new();
    BIO *keybio;
    keybio = BIO_new_mem_buf(pri_key, key_size);

    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    if (rsa == NULL)
    {
        unsigned long err = ERR_get_error();  //获取错误号
        char err_msg[1024] = {0};
        ERR_error_string(err, err_msg);  // 格式：error:errId:库:函数:原因
        printf("err msg: err:%ld, msg:%s\n", err, err_msg);

        BIO_free_all(keybio);
        RSA_free(rsa);

        return 0;
    }

    // 获取RSA单次处理的最大长度
    int unit_data_len = RSA_size(rsa);

    if (cipher_len % unit_data_len)
    {
        printf("cipher text length error!\r\n");

        BIO_free_all(keybio);
        RSA_free(rsa);

        return 0;
    }

    // 对密文进行分段解密
    cipher_pos = 0;
    while (cipher_pos < cipher_len)
    {

        //int RSA_private_decrypt(int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
        ret = RSA_private_decrypt(unit_data_len, (const unsigned char *)(cipher_text + cipher_pos), (unsigned char *)(plain_text + plain_pos), rsa, RSA_PKCS1_OAEP_PADDING);
        if (ret < 0)
        {
            plain_pos = 0;
            break;
        }

        cipher_pos += unit_data_len;
        plain_pos += ret;
    }

    // 释放内存
    BIO_free_all(keybio);
    RSA_free(rsa);

    return plain_pos;
}

int RSAHelper::calcDataEncryptLegth(int text_len, int key_bits)
{
    int unit_len = key_bits / 8;
    int block_len = unit_len - 41;
    int block_cnt = (text_len + block_len - 1) / block_len;

    return (block_cnt * unit_len);
}

#ifndef __ARM_EABI__
#pragma GCC diagnostic pop
#endif
