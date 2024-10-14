#include <iostream>

#include "parser.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <regex>
#include <memory>
#include "client.h"
#include "clientWithoutTLS.h"

#include "error.h"

using namespace std;

string receiveFromServer(int sock, char *buffer, int bufferSize) {
    std::memset(buffer, 0, bufferSize);
    ssize_t bytesReceived = recv(sock, buffer, bufferSize - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        string response = buffer;
        response = regex_replace(response, regex("\r"), "");
        return response;
    }

    return "No response from server or connection closed.";
}

int main(int argc, char **argv) {
    cout << "Hello, World!" << endl;
    Parser parser(argc, argv);
    parser.parse();
    parser.to_String();


    unique_ptr<Client> imap = make_unique<ClientWithoutTLS>();

    imap->connect(parser.getServer(), parser.getPort());

    cout << "go to sleep" << endl;
    sleep(5);
    cout << "wake up" << endl;

    imap->disconnect();

    // Send and receive data over IMAP connection
    // const char *login_command = "a1 LOGIN\r\n";
    // send(sock, login_command, strlen(login_command), 0);
    /*
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
    */

    return 0;
}
