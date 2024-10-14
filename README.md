# IMAP Client with TLS

The objective is to implement a **simple IMAP client** with **TLS support** that can connect to an IMAP server, authenticate a user, and retrieve email data securely.

## TODO/REDO the rest

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [License](#license)

## Overview
This IMAP client supports encrypted communication via **TLS/SSL** to ensure the privacy of user credentials and messages during transmission. It interacts with IMAP servers to perform basic operations, such as:
- User authentication
- Listing available mailboxes (folders)
- Fetching and displaying emails

## Features
- Secure IMAP communication using **TLS/SSL**.
- Basic IMAP commands support (e.g., `LOGIN`, `LIST`, `FETCH`).
- User-friendly error handling and output.

## Prerequisites
Before running or building the project, ensure you have the following dependencies installed:
- **CMake** (for C/C++ projects)
- **OpenSSL** library (for TLS support)
- A compatible **IMAP server** to connect to (e.g., Gmail, Outlook, etc.)

## TODO the rest