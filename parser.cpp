/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Argument processing implementation.
 * @author Denys Dumych <xdumyc00>
 */

#include "parser.h"

#include <iostream>
#include <ostream>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <regex>

#include "error.h"

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
    username = "";
    password = "";

    this->argc = argc;
    this->argv = argv;
}

void Parser::parse() {
    if (argc < 6) {
        std::cerr << "Wrong set of arguments." << std::endl;
        showHelp();
        exit(1);
    }

    server = argv[1];
    int opt;

    while ((opt = getopt(argc, argv, "p:Ta:c:C:nhb:o:H")) != -1) {
        switch (opt) {
            case 'p': {
                port = atoi(optarg);
                if (port <= 0) {
                    std::cerr << "Invalid port number." << std::endl;
                    showHelp();
                    exit(1);
                }
                break;
            }
            case 'T': {
                use_tls = true;
                port = 993;
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
            case 'H': {
                showHelp();
                exit(0);
            }
            default: {
                cerr << "Unknown argument." << endl;
                showHelp();
                exit(1);
            }
        }
    }

    if (auth_file.empty()) {
        cerr << "Authentication file (-a) is required." << endl;
        showHelp();
        exit(1);
    }

    if (output_dir.empty()) {
        cerr << "Output directory (-o) is required." << endl;
        showHelp();
        exit(1);
    }
}

void Parser::loadAuthData() {
    ifstream in(auth_file);

    string line;

    while (getline(in, line)) {
        istringstream iss(line);
        string key, eq, value;

        iss >> key >> eq >> std::ws;
        getline(iss, value);

        if (eq != "=") {
            new error("Invalid auth file format: expected 'key = value' in line: " + line);
        }

        if (key == "username") {
            this->username = regex_replace(value, regex("\r"), "");
        } else if (key == "password") {
            this->password = regex_replace(value, regex("\r"), "");
        } else {
            new error("Invalid auth file format: expected 'username' or 'password'");
        }
    }
}

void Parser::showHelp() {
    std::cout <<
            "Usage: ./imapcl server [-p port] [-T [-c certfile] [-C certaddr]] [-n] [-h] -a auth_file [-b MAILBOX] -o out_dir\n"
            << "Options:\n"
            << "  server            The IP address or domain name of the server (required).\n"
            << "  -p port           Specify the server port. Default: 143 (IMAP), 993 (IMAPS if -T is used).\n"
            << "  -T                Enable encryption (IMAPS). Default: unencrypted IMAP.\n"
            << "  -c certfile       File with SSL/TLS certificates for server verification.\n"
            << "  -C certaddr       Directory with SSL/TLS certificates. Default: /etc/ssl/certs.\n"
            << "  -n                Only process new messages.\n"
            << "  -h                Download only message headers.\n"
            << "  -a auth_file      Path to the authentication file (required).\n"
            << "  -b MAILBOX        Specify the mailbox to use. Default: INBOX.\n"
            << "  -o out_dir        Specify the output directory for downloaded messages (required).\n"
            << std::endl;
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

string Parser::getMailbox() {
    return mailbox;
}

string Parser::getOutputDir() {
    return output_dir;
}

std::string Parser::getUsername() {
    return username;
}

string Parser::getPassword() {
    return password;
}
