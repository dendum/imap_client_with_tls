/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Client without encryption implementation.
 * @author Denys Dumych <xdumyc00>
 */

#include <iostream>
#include "clientWithoutTLS.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <filesystem>

#include "error.h"

using namespace std;

/**
 * Establishes a connection to the server without using TLS encryption.
 *
 * Function creates a socket, resolves the server address, and attempts
 * to connect to the given server and port. If any step fails, it cleans up
 * and call an error message.
 *
 * @param server The server address to connect to.
 * @param port The port number to use for the connection.
 */
void ClientWithoutTLS::connect(const string &server, int port) {
    hostent *host;
    sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cleanup();
        new error("Socket creation failed.");
    }

    if ((host = gethostbyname(server.c_str())) == nullptr) {
        cleanup();
        new error("Host does not exist.");
    }

    auto **addresses = (in_addr **) host->h_addr_list;
    server_addr.sin_addr = *addresses[0];
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (::connect(sock, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cleanup();
        new error("Connection failed.");
    }

    string response = this->receiveFromServer();
    if (!isOkResponse(response)) {
        cleanup();
        new error("Connection failed(response).");
    }
}

void ClientWithoutTLS::cleanup() {
    if (sock != -1) {
        ::close(sock);
        sock = -1;
    }
}

void ClientWithoutTLS::send(const string &message) {
    if (::send(sock, message.c_str(), message.size(), 0) < 0) {
        cleanup();
        new error("Send failed.");
    }
}

string ClientWithoutTLS::receiveFromServer() {
    char buffer[8192] = {};

    if (::recv(sock, buffer, 8192, 0) < 0) {
        cleanup();
        new error("Receive failed.");
    }

    string response = buffer;
    return response;
}

ClientWithoutTLS::~ClientWithoutTLS() {
    ClientWithoutTLS::cleanup();
}
