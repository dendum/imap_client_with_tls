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
#include "clientWithTLS.h"

#include "error.h"

using namespace std;

void manualControl(ClientWithoutTLS &imap) {
    string input;
    while (true) {
        cout << "\nCommand> ";
        getline(cin, input);

        string formattedData = input + "\r\n";
        imap.send(formattedData);

        string response = imap.receiveFromServer();
        response = regex_replace(response, regex("\r"), "");
        cout << response;

        if (input.find("LOGOUT") != std::string::npos) break;
    }
}

int main(int argc, char **argv) {
    cout << "Hello, World!" << endl;
    Parser parser(argc, argv);
    parser.parse();
    parser.loadAuthData();
    // parser.to_String();

    // TODO: errors checker/catcher

    // unique_ptr<Client> imap = make_unique<ClientWithoutTLS>();
    if (parser.useTLS()) {
        /*** IMAP with TLS ***/
        clientWithTLS imaps;

        cout << "SECURED" << endl;
        cout << "Connecting to server..." << endl;
        imaps.connect(parser.getServer(), parser.getPort(), parser.getCertfile(), parser.getCertdir());
        cout << "\nLogging..." << endl;
        imaps.login(parser.getUsername(), parser.getPassword());
        cout << "\nSetting up..." << endl;
        imaps.selectMailbox(parser.getMailbox());
        cout << "\nGetting messages..." << endl;
        imaps.getMessages(parser.getOutputDir(), parser.onlyHeaders(), parser.onlyNew());
        cout << "\nLogging out..." << endl;
        imaps.logout();

    } else {
        /*** IMAP without TLS ***/
        ClientWithoutTLS imap;

        cout << "Connecting to server..." << endl;
        imap.connect(parser.getServer(), parser.getPort());
        cout << "\nLogging..." << endl;
        imap.login(parser.getUsername(), parser.getPassword());
        cout << "\nSetting up..." << endl;
        imap.selectMailbox(parser.getMailbox());
        cout << "\nGetting messages..." << endl;
        imap.getMessages(parser.getOutputDir(), parser.onlyHeaders(), parser.onlyNew());
        cout << "\nLogging out..." << endl;
        imap.logout();
    }

    return 0;
}
