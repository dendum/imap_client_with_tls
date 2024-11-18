//
// Created by Den on 14.10.2024.
//

#ifndef CLIENTWITHOUTTLS_H
#define CLIENTWITHOUTTLS_H

#include <string>

#include "client.h"

class ClientWithoutTLS : public Client {
public:
    void connect(const std::string &server, int port);

    void disconnect() override;

    void send(const std::string &message) override;

    std::string receiveFromServer() override;

    ~ClientWithoutTLS() override;

private:
    int sock = -1;
};


#endif //CLIENTWITHOUTTLS_H
