/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Client with encryption implementation.
 * @author Denys Dumych <xdumyc00>
 */

#include "clientWithTLS.h"

#include <fstream>
#include <iostream>

#include "error.h"

using namespace std;

void ClientWithTLS::connect(const std::string &server, int port,
                            const std::string &certfile, const std::string &certdir) {
    this->ctx = SSL_CTX_new(TLS_method());
    if (!ctx) {
        cleanup();
        new error("SSL context creation failed.");
    }

    // TODO: CHANGE TO DIR ON MERLIN!!!
    if (!certfile.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, certfile.c_str(), nullptr)) {
            cleanup();
            new error("Certificate file loading failed.");
        }
    } else if (certdir.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, nullptr, certdir.c_str())) {
            cleanup();
            new error("Certificate directory loading failed.");
        }
    } else {
        if (!SSL_CTX_set_default_verify_paths(ctx)) {
            cleanup();
            new error("Certificate loading failed.");
        }
    }

    this->myBIO = BIO_new_ssl_connect(this->ctx);
    SSL *ssl = nullptr;
    BIO_get_ssl(this->myBIO, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    string host = string(server).append(":").append(to_string(port));
    BIO_set_conn_hostname(this->myBIO, host.c_str());

    if (BIO_do_connect(this->myBIO) <= 0) {
        cleanup();
        new error("TLS connection failed.");
    }

    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        cleanup();
        new error("TLS connection failed(response).");
    }
}

void ClientWithTLS::cleanup() {
    if (this->myBIO) {
        BIO_free_all(this->myBIO);
        this->myBIO = nullptr;
    }
    if (this->ctx) {
        SSL_CTX_free(this->ctx);
        this->ctx = nullptr;
    }
}

void ClientWithTLS::send(const std::string &message) {
    if (BIO_write(this->myBIO, message.c_str(), message.size()) < 0) {
        cleanup();
        new error("Send failed.");
    }
}

std::string ClientWithTLS::receiveFromServer() {
    char buffer[8192] = {};

    if (BIO_read(this->myBIO, buffer, 8192) < 0) {
        cleanup();
        new error("Receive failed.");
    }

    string response = buffer;
    return response;
}

ClientWithTLS::~ClientWithTLS() {
    ClientWithTLS::cleanup();
}
