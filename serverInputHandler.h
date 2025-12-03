#ifndef SERVERINPUTHANDLER_H
#define SERVERINPUTHANDLER_H

#include <string>
#include <unordered_map>

extern const std::unordered_map<std::string, std::string> creds;
extern const int MAX_ATTEMPTS;
extern const int NUM_USERS;

std::string getLineButSecure(bool& validInput);

int getUserCredentials();  // Gets a username and password, validates them and then checks agains passwords 
int authenticateCredentials(std::string user, std::string password);    // helper function for getUserCredentials
void setStdinEcho(bool);
int serverInputHandler();   // main function for this

#endif