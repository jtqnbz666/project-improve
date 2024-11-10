#include "SecurityTool.h"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <sstream>
#include <ctime>
#include <cstring>

const std::string SecurityTool::KEY = "12345678901234567890123456789012";
const std::string SecurityTool::IV = "1234567890123456";

// Base64 encoding and decoding functions
std::string SecurityTool::base64Encode(const std::string &data) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Ignore newlines
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, data.data(), data.size());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string encodedData(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return encodedData;
}

std::string SecurityTool::base64Decode(const std::string &data) {
    BIO *bio, *b64;
    int decodeLen = data.size();
    char *buffer = (char *)malloc(decodeLen);
    memset(buffer, 0, decodeLen);

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Ignore newlines
    bio = BIO_new_mem_buf(data.data(), data.size());
    bio = BIO_push(b64, bio);

    int decodedSize = BIO_read(bio, buffer, decodeLen);
    std::string decodedData(buffer, decodedSize);

    BIO_free_all(bio);
    free(buffer);

    return decodedData;
}

// Encryption and decryption functions
std::string SecurityTool::encrypt(const std::string& plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)KEY.c_str(), (unsigned char*)IV.c_str());

    int len;
    int ciphertext_len = plaintext.size() + AES_BLOCK_SIZE;
    std::string ciphertext(ciphertext_len, '\0');
    
    EVP_EncryptUpdate(ctx, (unsigned char*)&ciphertext[0], &len, (const unsigned char*)plaintext.c_str(), plaintext.size());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, (unsigned char*)&ciphertext[0] + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);

    return base64Encode(ciphertext);
}

std::string SecurityTool::decrypt(const std::string& ciphertext) {
    std::string decodedCiphertext = base64Decode(ciphertext);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)KEY.c_str(), (unsigned char*)IV.c_str());

    int len;
    int plaintext_len = decodedCiphertext.size();
    std::string plaintext(plaintext_len, '\0');

    EVP_DecryptUpdate(ctx, (unsigned char*)&plaintext[0], &len, (const unsigned char*)decodedCiphertext.c_str(), decodedCiphertext.size());
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, (unsigned char*)&plaintext[0] + len, &len) <= 0) {
        printf("decrypt error\n");
        EVP_CIPHER_CTX_free(ctx);
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);

    return plaintext;
}

std::string SecurityTool::generateToken(const std::string& uid, const std::string& loginType, const std::string& passwd) {
    time_t now = time(0);
    std::string tokenData = base64Encode(loginType) + "|" + base64Encode(uid) + "|" + std::to_string(now) + "|" + base64Encode(passwd);
    return encrypt(tokenData);
}

bool SecurityTool::unpackToken(const std::string& token, 
    std::string& loginType,
    std::string& uid,
    std::string& passwd,
    std::string& timeStr
) {
    std::string decryptedToken;
    decryptedToken = decrypt(token);

    size_t firstSeparator = decryptedToken.find('|');
    size_t secondSeparator = decryptedToken.find('|', firstSeparator + 1);
    size_t thirdSeparator = decryptedToken.find('|', secondSeparator + 1);

    if (firstSeparator == std::string::npos || secondSeparator == std::string::npos || thirdSeparator == std::string::npos) {
        return false;
    }

    std::string loginTypeBase64 = decryptedToken.substr(0, firstSeparator);
    std::string uidBase64 = decryptedToken.substr(firstSeparator + 1, secondSeparator - firstSeparator - 1);
    timeStr = decryptedToken.substr(secondSeparator + 1, thirdSeparator - secondSeparator - 1);
    std::string passwdBase64 = decryptedToken.substr(thirdSeparator + 1);

    loginType = base64Decode(loginTypeBase64);
    uid = base64Decode(uidBase64);
    passwd = base64Decode(passwdBase64);

    return true;
}
