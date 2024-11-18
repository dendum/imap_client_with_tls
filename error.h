/**
* VUT FIT ISA 2024 project - IMAP client with TLS.
 *
 * @file Errors handling interface.
 * @author Denys Dumych <xdumyc00>
 */

#ifndef ERROR_H
#define ERROR_H
#include <string>

class error {
public:
    explicit error(const std::string &message);
};


#endif //ERROR_H
