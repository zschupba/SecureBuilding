#ifndef SERVERNETWORKING_H
#define SERVERNETWORKING_H

#include <string>

std::string processClientRequest(const std::string& requestData);

void startServerListener();

#endif