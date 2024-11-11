//
// Created by Den on 14.10.2024.
//

#ifndef CLIENTWITHOUTTLS_H
#define CLIENTWITHOUTTLS_H

#include <string>
#include <vector>

#include "client.h"

class ClientWithoutTLS : public Client {
public:
    void connect(const std::string &server, int port) override;

    void disconnect() override;

    void send(const std::string &message) override;

    std::string receiveFromServer() override;

    void login(const std::string &login, const std::string &password);

    void selectMailbox(const std::string &mailbox);

    void getMessages(const std::string &output_dir, bool headers_only);

    void parseUIDStringResponse(std::string &uidString);

    void loadMessage(int uid, const std::string &output_dir);

    std::string processMessage(int uid, bool message_part);

    void parseMessage(std::string &header);

    std::string formatMessageUID();

    void logout();

    ~ClientWithoutTLS() override;

private:
    bool headers_only = false;
    int message_count = 1;
    std::vector<int> UIDs = {};
};


#endif //CLIENTWITHOUTTLS_H
