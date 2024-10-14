#include <iostream>

#include "parser.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <regex>


#include "error.h"

using namespace std;

string receiveFromServer(int sock, char *buffer, int bufferSize) {
    // char buffer[4096];
    // cout << "SIZE: "<< sizeof(*buffer) << endl;
    std::memset(buffer, 0, bufferSize);
    ssize_t bytesReceived = recv(sock, buffer, bufferSize - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        string response = buffer;
        cout << "string size: " << response.size() << endl;
        response = regex_replace(response, regex("\r"), "");
        return response;
    } else {
        return "No response from server or connection closed.";
    }
}

int main(int argc, char **argv) {
    cout << "Hello, World!" << endl;
    Parser parser(argc, argv);
    parser.parse();
    parser.to_String();

    struct hostent *host;
    struct sockaddr_in server_addr;
    struct in_addr **addresses;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation failed." << endl;
        return -1;
    }
    cout << "Socket created." << endl;

    if ((host = gethostbyname(parser.getServer().c_str())) == NULL) {
        cerr << "Host does not exist." << endl;
        return -1;
    }

    addresses = (struct in_addr **) host->h_addr_list;

    for (int i = 0; addresses[i] != NULL; i++) {
        server_addr.sin_addr = *addresses[i];
        break;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(parser.getPort());

    if (::connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "Connection failed." << endl;
    }

    // Send and receive data over IMAP connection
    // const char *login_command = "a1 LOGIN\r\n";
    // send(sock, login_command, strlen(login_command), 0);


    char buffer[1024];
    string response = receiveFromServer(sock, buffer, sizeof(buffer));
    cout << response;

    string input;

    while (true) {
        cout << "Command> ";
        getline(cin, input);

        string formattedData = input + "\r\n";
        send(sock, formattedData.c_str(), formattedData.size(), 0);

        response = receiveFromServer(sock, buffer, sizeof(buffer));
        cout << response;

        if (input.find("LOGOUT") != std::string::npos) {
            break;
        }
    }

    close(sock);
    cout << "Connection closed.\n";

    return 0;
}
