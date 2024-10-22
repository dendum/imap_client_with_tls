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

    void getMessages();

    void parseUIDStringResponse(std::string &uidString);

    std::string formatMessageUID();

    ~ClientWithoutTLS() override;

private:
    int message_count = 1;
    std::vector<int> UIDs = {};
};


#endif //CLIENTWITHOUTTLS_H
