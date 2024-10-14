//
// Created by Den on 03.10.2024.
//

#ifndef ERROR_H
#define ERROR_H

enum IMAPCLError {
    OK = 0,
    E_ARGUMENTS = 1,
};

class error {
public:
    error(IMAPCLError);
};


#endif //ERROR_H
