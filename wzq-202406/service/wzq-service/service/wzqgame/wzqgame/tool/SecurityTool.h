#ifndef SECURITY_TOOL_H
#define SECURITY_TOOL_H

#include <string>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <sstream>
#include <ctime>
#include <cstring>
#include <iostream>

class SecurityTool {
public:
    static std::string encrypt(const std::string& plaintext);
    static std::string decrypt(const std::string& ciphertext);
    static std::string generateToken(const std::string& uid, const std::string& loginType, const std::string& passwd);
    static bool unpackToken(
    const std::string& token, 
    std::string& loginType,
    std::string& uid,
    std::string& passwd,
    std::string& timeStr);

private:
    static std::string base64Encode(const std::string& data);
    static std::string base64Decode(const std::string& data);

    static const std::string KEY;
    static const std::string IV;
};

#endif // SECURITY_TOOL_H
