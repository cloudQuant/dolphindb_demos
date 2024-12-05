#pragma once
#ifdef USE_OPENSSL
#include <openssl/rsa.h>
#endif
#include <string>
#include <vector>

namespace dolphindb {

#ifdef USE_OPENSSL

class Crypto {
public:
    Crypto(const std::string &publicKey);
    ~Crypto();
    std::string RSAEncrypt(const std::string &text) const;

private:
    void printOpenSSLError() const;
    std::string Base64Encode(const std::vector<unsigned char> &text) const;

private:
    RSA *rsa_;
    BIO *keyBio_;
};

#endif

}
