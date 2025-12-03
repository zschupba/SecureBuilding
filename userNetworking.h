#ifndef USERNETWORKING_H
#define USERNETWORKING_H

#include <string>

bool sendRequestToServer(const std::string& requestData, std::string& serverResponse);

#endif