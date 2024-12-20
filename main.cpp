/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Main file.
 * @author Denys Dumych <xdumyc00>
 */

#include <iostream>
#include "parser.h"
#include "client.h"
#include "clientWithoutTLS.h"
#include "clientWithTLS.h"

using namespace std;

int main(int argc, char **argv) {
    // Create a Parser object to handle command-line arguments
    Parser parser(argc, argv);
    parser.parse();
    parser.loadAuthData();

    // Check if TLS (encrypted connection) is enabled
    if (parser.useTLS()) {
        /*** IMAP with TLS ***/
        ClientWithTLS imaps;

        imaps.connect(parser.getServer(), parser.getPort(), parser.getCertfile(), parser.getCertdir());
        imaps.login(parser.getUsername(), parser.getPassword());
        imaps.selectMailbox(parser.getMailbox());
        imaps.getMessages(parser.getOutputDir(), parser.onlyHeaders(), parser.onlyNew());
        imaps.logout();

    } else {
        /*** IMAP without TLS ***/
        ClientWithoutTLS imap;

        imap.connect(parser.getServer(), parser.getPort());
        imap.login(parser.getUsername(), parser.getPassword());
        imap.selectMailbox(parser.getMailbox());
        imap.getMessages(parser.getOutputDir(), parser.onlyHeaders(), parser.onlyNew());
        imap.logout();
    }

    return 0;
}
