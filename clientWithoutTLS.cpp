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
#include <filesystem>
#include <fstream>
#include <regex>
#include <iomanip>
#include <sstream>

using namespace std;

// back up func // delete it
void print_for_personal_use(string response) {
    response = regex_replace(response, regex("\r"), "");
    cout << response;
}

// inspired by:
// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// https://stackoverflow.com/questions/44973435/stdptr-fun-replacement-for-c17/44973498#44973498
// https://cplusplus.com/reference/cctype/isspace/
inline void ltrim(string &str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
}

inline void rtrim(string &str) {
    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), str.end());
}

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
    cout << ">> ";
    char a;
    for (long unsigned int i = 0; i < message.size(); i++) {
        a = message[i];
        if (a == '\r')
            continue;
        cout << message[i];
    }
    if (::send(sock, message.c_str(), message.size(), 0) < 0) {
        cerr << "Send failed." << endl;
        // TODO: errors catcher
        exit(0);
    }
}

string ClientWithoutTLS::receiveFromServer() {
    char buffer[16384] = {};

    if (::recv(sock, buffer, 16384, 0) < 0) {
        cerr << "Receive failed.return."
                "No response from server or connection closed." << endl;
        // TODO: errors catcher
        exit(0);
    }

    string response = buffer;
    return response;
}

void ClientWithoutTLS::login(const string &username, const string &password) {
    string message = formatMessageUID();
    message.append(" LOGIN ").append(username).append(" ").append(password).append("\r\n");

    send(message);

    string response = this->receiveFromServer();
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
    // TODO check OK response
    // if(message.find("OK") != -1)
}

void ClientWithoutTLS::getMessages(const string &output_dir, bool headers_only, bool only_new) {
    this->headers_only = headers_only;
    this->only_new = only_new;
    // get uid info
    string message = formatMessageUID();
    message.append(" UID SEARCH ");
    if (this->only_new) {
        message.append("UNSEEN");
    } else {
        message.append("ALL");
    }
    message.append("\r\n");
    send(message);

    string response = this->receiveFromServer();
    // TODO check OK response
    // if(message.find("OK") != -1)

    // parse uids
    parseUIDStringResponse(response);
    // cout << "Our UIDS: ";
    // for (size_t it = 0; it < UIDs.size(); it++) {
        // cout << UIDs.at(it) << " ";
    // }
    // cout << endl;

    for (const int uid: UIDs) {
        loadMessage(uid, output_dir);
    }
}

void ClientWithoutTLS::parseUIDStringResponse(string &uidString) {
    string searchLinePrefix = "* SEARCH ";

    size_t pos = uidString.find(searchLinePrefix);
    if (pos != string::npos) {
        string uid = uidString.substr(pos + searchLinePrefix.size());
        stringstream ss(uid);

        int uidInt;
        while (ss >> uidInt) {
            UIDs.push_back(uidInt);
        }
    }
}

void ClientWithoutTLS::loadMessage(int uid, const string &output_dir) {
    cout << endl;
    string result, header, body;

    header = processMessage(uid, true);
    if (!this->headers_only) {
        body = processMessage(uid, false);
    }

    ofstream file(output_dir + "/msg" + to_string(uid) + ".txt");
    if (this->headers_only) {
        file << header << '\n';
    } else {
        file << header << "\n\n" << body << '\n';
    }
}

string ClientWithoutTLS::processMessage(int uid, bool message_part) {
    string message = formatMessageUID();
    if (message_part) {
        message.append(" UID FETCH ").append(to_string(uid)).append(
            " BODY.PEEK[HEADER.FIELDS (DATE FROM TO SUBJECT Message-ID)]").append("\r\n");
    } else {
        message.append(" UID FETCH ").append(to_string(uid)).append(" BODY.PEEK[1]").append("\r\n");
    }
    send(message);
    string response = this->receiveFromServer();
    // TODO check OK response
    parseMessage(response);
    ltrim(response);
    rtrim(response);
    return response;
}

void ClientWithoutTLS::parseMessage(std::string &message) {
    // message.erase(0, message.find("\r\n") + 2);
    // message.erase(message.rfind(')'));
    size_t pos = message.find("\r\n");
    if (pos != std::string::npos) {
        message.erase(0, pos + 2);

        size_t lastParen = message.rfind(')');
        if (lastParen != std::string::npos) {
            message.erase(lastParen);
        }
    }
}

string ClientWithoutTLS::formatMessageUID() {
    // string maybe = std::format("A{:03}", message_count); !! c++20 !! somehow wsl problems??
    ostringstream oss;
    oss << "A" << std::setw(3) << setfill('0') << this->message_count++;
    return oss.str();
}

void ClientWithoutTLS::logout() {
    string message = formatMessageUID();
    message.append(" LOGOUT ").append("\r\n");

    send(message);
    string response = this->receiveFromServer();
    // TODO check OK response

    disconnect();
}


ClientWithoutTLS::~ClientWithoutTLS() {
    ClientWithoutTLS::disconnect();
}
