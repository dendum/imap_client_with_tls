//
// Created by Den on 14.10.2024.
//

#include <iostream>
#include "clientWithoutTLS.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <regex>
#include <iomanip>
#include <sstream>

using namespace std;

void ClientWithoutTLS::connect(const std::string &server, int port) {
    hostent *host;
    sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation failed." << endl;
        // TODO: errors catcher
        exit(0);
    }

    if ((host = gethostbyname(server.c_str())) == NULL) {
        cerr << "Host does not exist." << endl;
        // TODO: errors catcher
        exit(0);
    }

    auto **addresses = (in_addr **) host->h_addr_list;
    server_addr.sin_addr = *addresses[0];
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (::connect(sock, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "Connection failed." << endl;
        // TODO: errors catcher
        exit(0);
    }

    string response = this->receiveFromServer();
    cout << response;
    // if(message.find("+OK ") != -1)
    if (response.find("OK") == string::npos) {
        cerr << "Connection failed(response)";
    }
}

void ClientWithoutTLS::disconnect() {
    if (sock != -1) {
        ::close(sock);
        sock = -1;
        cout << "disconnected" << endl;
    }
}

void ClientWithoutTLS::send(const string &message) {
    if (::send(sock, message.c_str(), message.size(), 0) < 0) {
        cerr << "Send failed." << endl;
        // TODO: errors catcher
        exit(0);
    }
}

string ClientWithoutTLS::receiveFromServer() {
    char buffer[8192] = {};

    if (::recv(sock, buffer, 8192, 0) < 0) {
        cerr << "Receive failed.return."
                "No response from server or connection closed." << endl;
        // TODO: errors catcher
        exit(0);
    }

    string response = regex_replace(buffer, regex("\r"), "");
    return response;
}

void ClientWithoutTLS::login(const string &username, const string &password) {
    string message = formatMessageUID();
    message.append(" LOGIN ").append(username).append(" ").append(password).append("\r\n");

    send(message);

    string response = this->receiveFromServer();
    cout << response;
    // TODO check OK response
    // if(message.find("OK") != -1)
    // if (response.find("Logged in") == string::npos) {
        // cerr << "Logging failed." << endl;
    // }
}

void ClientWithoutTLS::selectMailbox(const std::string &mailbox) {
    string message = formatMessageUID();
    message.append(" SELECT ").append(mailbox).append(" ").append("\r\n");
    send(message);

    string response = this->receiveFromServer();
    cout << response;
    // TODO check OK response
    // if(message.find("OK") != -1)
}

void ClientWithoutTLS::getMessages() {
    string message = formatMessageUID();
    message.append(" SEARCH ALL").append("\r\n");
    // TODO flags i guess?
    send(message);

    string response = this->receiveFromServer();
    cout << response;
    // TODO check OK response
    // if(message.find("OK") != -1)
}

string ClientWithoutTLS::formatMessageUID() {
    // string maybe = std::format("A{:03}", message_count); !! c++20 !! somehow wsl problems??
    ostringstream oss;
    oss << "A" << std::setw(3) << setfill('0') << this->message_count++;
    return oss.str();
}


ClientWithoutTLS::~ClientWithoutTLS() {
    ClientWithoutTLS::disconnect();
}
