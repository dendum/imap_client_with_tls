/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Base client interface.
 * @author Denys Dumych <xdumyc00>
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

/**
 * Base class for the IMAP client interface, which defines common operations for all IMAP clients.
 */
class Client {
public:
    virtual void cleanup() = 0;

    virtual void send(const std::string &message) = 0;

    virtual std::string receiveFromServer() = 0;

    void login(const std::string &username, const std::string &password);

    void selectMailbox(const std::string &mailbox);

    void getMessages(const std::string &output_dir, bool headers_only, bool only_new);

    void parseUIDStringResponse(std::string &uidString);

    void loadMessage(int uid, const std::string &output_dir);

    std::string processMessage(int uid, bool message_part);

    void parseMessage(std::string &message);

    std::string formatMessageUID();

    bool isOkResponse(const std::string &response);

    void ltrim(std::string &str);

    void rtrim(std::string &str);

    void logout();

    virtual ~Client() = default;

protected:
    int message_count = 1;
    bool headers_only = false;
    bool only_new = false;
    std::string mailbox;
    std::vector<int> UIDs = {};
};

#endif //CLIENT_H
