//
// Created by Den on 03.10.2024.
//

#include "parser.h"

#include <iostream>
#include <ostream>
#include <unistd.h>
#include <cstdlib>


Parser::Parser(int argc, char **argv) {
    server = "";
    port = 143;
    use_tls = false;
    certfile = "";
    certdir = "/etc/ssl/certs";
    only_new = false;
    headers_only = false;
    auth_file = "";
    mailbox = "INBOX";
    output_dir = "";

    this->argc = argc;
    this->argv = argv;
}

void Parser::parse() {
    // Použití: imapcl server [-p port] [-T [-c certfile] [-C certaddr]] [-n] [-h] -a auth_file [-b MAILBOX] -o out_dir
    int opt;
    server = argv[1];

    while ((opt = getopt(argc, argv, "p:Ta:c:C:nhb:o:")) != -1) {
        switch (opt) {
            case 'p': {
                port = atoi(optarg);
                break;
            }
            case 'T': {
                use_tls = true;
                break;
            }
            case 'a': {
                auth_file = optarg;
                break;
            }
            case 'c': {
                certfile = optarg;
                break;
            }
            case 'C': {
                certdir = optarg;
                break;
            }
            case 'n': {
                only_new = true;
                break;
            }
            case 'h': {
                headers_only = true;
                break;
            }
            case 'b': {
                mailbox = optarg;
                break;
            }
            case 'o': {
                output_dir = optarg;
                break;
            }
            default: ;
        }
    }
}

string Parser::getServer() {
    return server;
}

int Parser::getPort() {
    return port;
}

bool Parser::useTLS() {
    return use_tls;
}

string Parser::getCertfile() {
    return certfile;
}

string Parser::getCertdir() {
    return certdir;
}

bool Parser::onlyNew() {
    return only_new;
}

bool Parser::onlyHeaders() {
    return headers_only;
}

string Parser::getAuthFile() {
    return auth_file;
}

string Parser::getMailbox() {
    return mailbox;
}

string Parser::getOutputDir() {
    return output_dir;
}

void Parser::to_String() {
    cout << "======= Args Info =======" << endl;
    printf("Count of arguments: %d\n", argc);
    printf("server: %s\n", getServer().c_str());
    printf("Port: %d\n", getPort());
    printf("Use TLS: %s\n", useTLS() ? "true" : "false");
    printf("Certfile: %s\n", getCertfile().c_str());
    printf("Certdir: %s\n", getCertdir().c_str());
    printf("Only new: %s\n", onlyNew() ? "true" : "false");
    printf("Only new: %s\n", onlyHeaders() ? "true" : "false");
    printf("Auth file: %s\n", getAuthFile().c_str());
    printf("Mailbox: %s\n", getMailbox().c_str());
    printf("Output dir: %s\n", getOutputDir().c_str());
    cout << "======= Args Info End =======" << endl;
}

