//
// Created by Den on 14.10.2024.
//

#ifndef CLIENT_H
#define CLIENT_H

class Client {
public:
    virtual void connect(const std::string &server, int port) = 0;

    virtual void disconnect() = 0;

    virtual void send(const std::string &string) = 0;

    virtual ~Client() = default;

protected:
    int sock{};
};

#endif //CLIENT_H
