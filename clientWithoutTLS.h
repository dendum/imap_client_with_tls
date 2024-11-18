/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Client without encryption interface.
 * @author Denys Dumych <xdumyc00>
 */

#ifndef CLIENTWITHOUTTLS_H
#define CLIENTWITHOUTTLS_H

#include <string>
#include "client.h"

class ClientWithoutTLS : public Client {
public:
    void connect(const std::string &server, int port);

    void cleanup() override;

    void send(const std::string &message) override;

    std::string receiveFromServer() override;

    ~ClientWithoutTLS() override;

private:
    int sock = -1;
};


#endif //CLIENTWITHOUTTLS_H
