//
// Created by Den on 03.10.2024.
//

#ifndef PARSER_H
#define PARSER_H
#include <string>

using namespace std;

class Parser {
public:
    Parser(int argc, char **argv);
    void parse();
    void loadAuthData();
    void showHelp();
    string getServer();
    int getPort();
    bool useTLS();
    string getCertfile();
    string getCertdir();
    bool onlyNew();
    bool onlyHeaders();
    string getMailbox();
    string getOutputDir();
    std::string getUsername();
    string getPassword();

private:
    unsigned int argc;
    char **argv;

    string server;
    int port;
    bool use_tls = false;
    string certfile;
    string certdir = "/etc/ssl/certs";
    bool only_new = false;
    bool headers_only = false;
    string auth_file;
    string mailbox = "INBOX";
    string output_dir;
    string username;
    string password;
};


#endif //PARSER_H
