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

void manualControl(ClientWithoutTLS &imap) {
    string input;
    while (true) {
        cout << "Command> ";
        getline(cin, input);

        string formattedData = input + "\r\n";
        imap.send(formattedData);
        cout << imap.receiveFromServer();

        if (input.find("LOGOUT") != std::string::npos) break;
    }
}

int main(int argc, char **argv) {
    cout << "Hello, World!" << endl;
    Parser parser(argc, argv);
    parser.parse();
    parser.loadAuthData();
    // parser.to_String();

    /*** IMAP without TLS ***/
    // TODO: errors checker/catcher

    // unique_ptr<Client> imap = make_unique<ClientWithoutTLS>();
    ClientWithoutTLS imap;

    cout << "Connecting to server..." << endl;
    imap.connect(parser.getServer(), parser.getPort());
    cout << "Logging..." << endl;
    imap.login(parser.getUsername(), parser.getPassword());
    cout << "Setting up..." << endl;
    imap.selectMailbox(parser.getMailbox());
    cout << "Getting messages..." << endl;
    imap.getMessages();

    manualControl(imap);

    imap.disconnect();

    return 0;
}
