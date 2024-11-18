/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Errors handling implementation.
 * @author Denys Dumych <xdumyc00>
 */

#include "error.h"

#include <iostream>
#include <string>

using namespace std;

error::error(const std::string &message) {
    cerr << message << endl;
    exit(1);
}
