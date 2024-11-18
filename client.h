//
// Created by Den on 14.10.2024.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

class Client {
public:
    // void connect(const std::string &server, int port);

    virtual void disconnect() = 0;

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
