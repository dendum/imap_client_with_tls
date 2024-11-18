# ISA Síťové aplikace a správa sítí - Projekt
### IMAP Client with TLS

### Autor: Denys Dumych <xharmi00>

### Popis programu

The program connects to an IMAP server and downloads email messages. It supports both encrypted connections via TLS (IMAPS) and unencrypted connections (IMAP). The user can specify whether to download only message headers or full messages, and whether to fetch only new (unread) emails. The program also allows selecting an output directory for storing downloaded messages and supports authentication via a user credentials file.

### Compile
Run `make` in the project directory to compile the source files into an executable. The Makefile will automatically compile the .cpp files into object files and link them into the final executable imapcl.

### Usage
`./imapcl server [-p <port>] [-T [-c <certfile>] [-C <certaddr>]] [-n] [-h] -a <auth_file> [-b <MAILBOX>] -o <out_dir>`

- `server`          The IP address or domain name of the server (required).
- `-p <port>`       Specify the server port. Default: 143 (IMAP), 993 (IMAPS if -T is used).
- `-T`              Enable encryption (IMAPS). Default: unencrypted IMAP.
- `-c <certfile>`   File with SSL/TLS certificates for server verification.
- `-C <certaddr>`   Directory with SSL/TLS certificates. Default: /etc/ssl/certs.
- `-n`              Only process new messages.
- `-h`              Download only message headers.
- `-a <auth_file>`  Path to the authentication file (required).
- `-b <MAILBOX>`    Specify the mailbox to use. Default: INBOX.
- `-o <out_dir>`    Specify the output directory for downloaded messages (required).
- `-H`              Show help information for users.

### List of files
- `./CMakeLists.txt`
- `./Makefile`
- `./README.md`
- `./manual.pdf`
- `./client.cpp`
- `./client.h`
- `./clientWithoutTLS.cpp`
- `./clientWithoutTLS.h`
- `./clientWithTLS.cpp`
- `./clientWithTLS.h`
- `./error.cpp`
- `./error.h`
- `./parser.cpp`
- `./parser.h`
- `./main.cpp`
