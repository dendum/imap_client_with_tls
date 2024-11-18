/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Base client implementation.
 * @author Denys Dumych <xdumyc00>
 */

#include "client.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>

#include "error.h"

using namespace std;

void Client::login(const string &username, const string &password) {
    string message = formatMessageUID();
    message.append(" LOGIN ").append(username).append(" ").append(password).append("\r\n");

    send(message);

    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        new error("Login failed");
    }
}

void Client::selectMailbox(const string &mailbox) {
    this->mailbox = mailbox;
    string message = formatMessageUID();
    message.append(" SELECT ").append(mailbox).append(" ").append("\r\n");
    send(message);

    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        new error("Select mailbox failed");
    }
}

void Client::getMessages(const string &output_dir, bool headers_only, bool only_new) {
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
    if (!isOkResponse(response)) {
        new error("Mailbox UID SEARCH failed");
    }

    parseUIDStringResponse(response);
    for (const int uid: UIDs) {
        loadMessage(uid, output_dir);
    }

    if (only_new) {
        cout << "Downloaded " << UIDs.size() <<
                " new messages from the " << this->mailbox << " mailbox." << endl;
    } else {
        cout << "Downloaded " << UIDs.size() <<
                " messages from the " << this->mailbox << " mailbox." << endl;
    }
}

void Client::parseUIDStringResponse(string &uidString) {
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

void Client::loadMessage(int uid, const string &output_dir) {
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

string Client::processMessage(int uid, bool message_part) {
    string message = formatMessageUID();
    if (message_part) {
        message.append(" UID FETCH ").append(to_string(uid)).append(
            " BODY.PEEK[HEADER.FIELDS (DATE FROM TO SUBJECT Message-ID)]").append("\r\n");
    } else {
        message.append(" UID FETCH ").append(to_string(uid)).append(" BODY.PEEK[1]").append("\r\n");
    }
    send(message);
    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        new error("Message fetch failed");
    }
    parseMessage(response);
    ltrim(response);
    rtrim(response);
    return response;
}

void Client::parseMessage(string &message) {
    // message.erase(0, message.find("\r\n") + 2);
    // message.erase(message.rfind(')'));
    size_t pos = message.find("\r\n");
    if (pos != string::npos) {
        message.erase(0, pos + 2);

        size_t lastParen = message.rfind(')');
        if (lastParen != string::npos) {
            message.erase(lastParen);
        }
    }
}

string Client::formatMessageUID() {
    ostringstream oss;
    oss << "A" << setw(3) << setfill('0') << this->message_count++;
    return oss.str();
}

inline bool Client::isOkResponse(const string &response) {
    return response.find("OK") != -1;
}

inline void Client::ltrim(string &str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
}

inline void Client::rtrim(string &str) {
    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), str.end());
}

void Client::logout() {
    string message = formatMessageUID();
    message.append(" LOGOUT ").append("\r\n");

    send(message);
    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        new error("Logging out failed");
    }

    cleanup();
}
