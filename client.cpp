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

/**
 * Logs into the IMAP server with the provided username and password.
 * Function sends a LOGIN command to the server and waits for the response.
 *
 * @param username The username for login.
 * @param password The password for login.
 */
void Client::login(const string &username, const string &password) {
    string message = formatMessageUID();
    message.append(" LOGIN ").append(username).append(" ").append(password).append("\r\n");

    send(message);

    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        new error("Login failed");
    }
}

/**
 * Selects the specified mailbox on the IMAP server.
 * Function sends a SELECT command to the server to select the mailbox.
 *
 * @param mailbox The name of the mailbox to be selected (default "INBOX").
 */
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

/**
 * Download messages from the selected mailbox on the IMAP server.
 * Function performs a UID SEARCH to find all or only new (unseen) messages,
 * then downloads and processes each message.
 *
 * @param output_dir The directory where the downloaded messages will be stored.
 * @param headers_only If true, only the headers of the messages will be downloaded.
 * @param only_new If true, only unseen (new) messages will be retrieved.
 */
void Client::getMessages(const string &output_dir, bool headers_only, bool only_new) {
    this->headers_only = headers_only;
    this->only_new = only_new;

    // Build the UID SEARCH command to fetch message UIDs
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
    // Loop through each UID and download the corresponding message
    for (const int uid: UIDs) {
        loadMessage(uid, output_dir);
    }

    // Output the number of messages downloaded
    if (only_new) {
        cout << "Downloaded " << UIDs.size() <<
                " new messages from the " << this->mailbox << " mailbox." << endl;
    } else {
        cout << "Downloaded " << UIDs.size() <<
                " messages from the " << this->mailbox << " mailbox." << endl;
    }
}

/**
 * Parses the response from the IMAP server to extract the UIDs of the messages.
 * The response format from the server begins with "* SEARCH", followed by a space-separated list of UIDs.
 *
 * @param uidString The string response received from the IMAP server containing the list of UIDs.
 */
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

/**
 * Loads a message from the IMAP server and saves it to a file in the specified output directory.
 * The message is saved as a text file with the message's UID as the filename. The file contains
 * either only the header or both the header and the body, depending on the user's choice.
 *
 * @param uid The unique identifier of the message to be loaded.
 * @param output_dir The directory where the message should be saved.
 */
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

/**
 * Processes an IMAP message by fetching either the header or the body of the message.
 *
 * @param uid The unique identifier of the message to fetch.
 * @param message_part A bool to fetch the header (true) or body (false) of the message.
 * @return The response from the server containing the requested message part (header or body).
 */
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

/**
 * Parses the IMAP server response message, removing unnecessary parts.
 *
 * @param message The message string to be parsed. It is modified in-place.
 */
void Client::parseMessage(string &message) {
    size_t pos = message.find("\r\n");
    if (pos != string::npos) {
        message.erase(0, pos + 2);

        size_t lastParen = message.rfind(')');
        if (lastParen != string::npos) {
            message.erase(lastParen);
        }
    }
}

/**
 * Function is used to generate a unique message ID for each IMAP command.
 * Formats and returns a message UID in the format "A###", where '###' is a
 * zero-padded 3-digit number starting from 001 and incrementing with each call.
 *
 * @return A string representing the formatted message UID.
 */
string Client::formatMessageUID() {
    ostringstream oss;
    oss << "A" << setw(3) << setfill('0') << this->message_count++;
    return oss.str();
}

/**
 * Checks if the response from the IMAP server is an "OK" response.
 *
 * @param response The response string from the server to be checked.
 * @return `true` if the response contains "OK", indicating success; `false` otherwise.
 */
inline bool Client::isOkResponse(const string &response) {
    return response.find("OK") != -1;
}

/**
 * Trims whitespace characters from a string.
 * Inspired by:
 * https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
 * https://stackoverflow.com/questions/44973435/stdptr-fun-replacement-for-c17/44973498#44973498
 * @param str The string from which whitespace characters will be removed.
 */
inline void Client::ltrim(string &str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
}

/**
 * Trims whitespace characters from a string.
 * Inspired by:
 * https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
 * https://stackoverflow.com/questions/44973435/stdptr-fun-replacement-for-c17/44973498#44973498
 * @param str The string from which whitespace characters will be removed.
 */
inline void Client::rtrim(string &str) {
    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), str.end());
}

/**
 * Sends a LOGOUT command to the IMAP server to terminate the session.
 */
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
