//
// Created by Den on 14.10.2024.
//

#ifndef CLIENTWITHOUTTLS_H
#define CLIENTWITHOUTTLS_H

#include <string>

#include "client.h"

class ClientWithoutTLS : public Client {
public:
    void connect(const std::string &server, int port) override;

    void disconnect() override;

    void send(const std::string &message) override;

    std::string receiveFromServer();

    ~ClientWithoutTLS() override;
};


#endif //CLIENTWITHOUTTLS_H
