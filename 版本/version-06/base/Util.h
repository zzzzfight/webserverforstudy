#pragma once

#include <string>
using namespace std;
// class Util{};
void handle_for_sigpipe();
int setSocketNonBlocking(int fd);
void setSocketNodelay(int fd);
void setSocketNoLinger(int fd);
void shutDownWR(int fd);
int socket_bind_listen(int port);

// void readn(int fd, string &buffer);
size_t readn(int fd, std::string &buffer);
