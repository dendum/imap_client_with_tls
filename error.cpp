//
// Created by Den on 03.10.2024.
//

#include "error.h"

#include <iostream>
#include <string>

using namespace std;

error::error(const IMAPCLError errorCode) {
    string errorMessage;
    switch (errorCode) {
        case OK:
            errorMessage = "Everything is fine";
            break;
        default:
            errorMessage = "Unknown error";
    }

    fprintf(stderr, " ERROR: %s\n", errorMessage.c_str());
    cout << "======= EXIT =======" << endl;
    exit(errorCode);
}
