//
// Created by Den on 15.11.2024.
//

#include "clientWithTLS.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <sstream>

using namespace std;

// back up func // delete it
void print_for_secure_personal_use(string response) {
    response = regex_replace(response, regex("\r"), "");
    cout << response;
}

void clientWithTLS::connect(const std::string &server, int port,
                            const std::string &certfile, const std::string &certdir) {
    this->ctx = SSL_CTX_new(TLS_method());
    // ctx error ???!

    if (!ctx) {
        cerr << "error ctx" << endl;
        // TODO: errors catcher
        exit(0);
    }

    // TODO: CHANGE TO DIR ON MERLIN!!!
    if (!certfile.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, certfile.c_str(), nullptr)) {
            cerr << "error cert file" << endl;
            // TODO: errors catcher
            exit(0);
        }
    } else if (certdir.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, nullptr, certdir.c_str())) {
            cerr << "error cert dir" << endl;
            // TODO: errors catcher
            exit(0);
        }
    } else {
        if (!SSL_CTX_set_default_verify_paths(ctx)) {
            cerr << "ERROR when setting default certification locations" << endl;
            exit(0);
        }
    }

    this->myBIO = BIO_new_ssl_connect(this->ctx);
    SSL *ssl = nullptr;
    BIO_get_ssl(this->myBIO, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    string host = string(server).append(":").append(to_string(port));
    BIO_set_conn_hostname(this->myBIO, host.c_str());

    if (BIO_do_connect(this->myBIO) <= 0) {
        cerr << "connection failed" << endl;
        // TODO: errors catcher
        exit(0);
    }


    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        cerr << "Connection failed(response)" << endl;
        // TODO: errors catcher
        exit(0);
    }
}

void clientWithTLS::disconnect() {
    // if (this->myBIO) BIO_free_all(this->myBIO);
    // if (this->mySSL) SSL_shutdown(this->mySSL);
    // if (this->ctx) SSL_CTX_free(this->ctx);
    // BIO_free_all(this->myBIO);
    // SSL_shutdown(this->mySSL);
    // SSL_free(this->mySSL);
    // SSL_CTX_free(this->ctx);
    if (this->myBIO) {
        BIO_free_all(this->myBIO);
        this->myBIO = nullptr;
    }
    if (this->ctx) {
        SSL_CTX_free(this->ctx);
        this->ctx = nullptr;
    }
    cout << "disconnected" << endl;
}

void clientWithTLS::send(const std::string &message) {
    // cout << ">> ";
    // char a;
    // for (long unsigned int i = 0; i < message.size(); i++) {
    // a = message[i];
    // if (a == '\r')
    // continue;
    // cout << message[i];
    // }
    // SSL_write(this->mySSL, message.c_str(), message.length());
    if (BIO_write(this->myBIO, message.c_str(), message.size()) < 0) {
        cerr << "Send failed." << endl;
        // TODO: errors catcher
        exit(0);
    }
}

std::string clientWithTLS::receiveFromServer() {
    char buffer[16384] = {};

    if (BIO_read(this->myBIO, buffer, 16384) < 0) {
        cerr << "Receive failed.return."
                "No response from server or connection closed." << endl;
        // TODO: errors catcher
        exit(0);
    }

    string response = buffer;
    return response;
}

clientWithTLS::~clientWithTLS() {
    clientWithTLS::disconnect();
}
