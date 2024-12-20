/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Client with encryption interface.
 * @author Denys Dumych <xdumyc00>
 */

#ifndef CLIENTWITHTLS_H
#define CLIENTWITHTLS_H
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/crypto.h>

#include "client.h"

class ClientWithTLS : public Client {
public:
    void connect(const std::string &server, int port, const std::string &certfile, const std::string &certdir);

    void cleanup() override;

    void send(const std::string &message) override;

    std::string receiveFromServer() override;

    ~ClientWithTLS() override;

private:
    SSL_CTX *ctx = nullptr;
    SSL *mySSL = nullptr;
    BIO *myBIO = nullptr;
};


#endif //CLIENTWITHTLS_H
