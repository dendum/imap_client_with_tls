//
// Created by Den on 03.10.2024.
//

#include "error.h"

#include <iostream>
#include <string>

using namespace std;

error::error(const std::string &message) {
    cerr << message << endl;
    exit(1);
}
