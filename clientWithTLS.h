//
// Created by Den on 15.11.2024.
//

#ifndef CLIENTWITHTLS_H
#define CLIENTWITHTLS_H
#include <string>
#include <vector>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>

#include "client.h"

class clientWithTLS : public Client {
public:
    void connect(const std::string &server, int port, const std::string &certfile, const std::string &certdir);

    void disconnect() override;

    void send(const std::string &message) override;

    std::string receiveFromServer() override;

    ~clientWithTLS() override;

private:
    SSL_CTX *ctx = nullptr;
    SSL *mySSL = nullptr;
    BIO *myBIO = nullptr;
};


#endif //CLIENTWITHTLS_H
