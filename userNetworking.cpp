//Zachary Schupbach
// 11/26/25
// This code does all the encrypting and network traffic on the user computer side
// passes a string from user to server, return a string from server to user

#include "userNetworking.cpp"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

const int PORT = 8080;
const std::string SECRET = "SECRET_KEY";
const std::string SERVER_IP = "127.0.0.1"; // !!! CHANGE THIS TO YOUR SERVER'S IP !!!

bool sendRequestToServer(const std::string& requestData, std::string& serverResponse) {
    int sock = 0;
    struct sockaddr_in servAddr;
    std::string message = SECRET + requestData;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error \n";
        return false;
    }
   
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
       
    if(inet_pton(AF_INET, SERVER_IP.c_str(), &servAddr.sin_addr)<=0) {
        std::cerr << "Invalid address/ Address not supported \n";
        return false;
    }
   
    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        std::cerr << "Connection Failed \n";
        return false;
    }
    
    send(sock, message.c_str(), message.length(), 0);
    
    char buffer[2048] = {0}; // Buffer large enough for the server's response (log file content)
    long valread = read(sock, buffer, sizeof(buffer) - 1);
    close(sock);

    if (valread > 0) {
        serverResponse = std::string(buffer);
        return true;
    } else {
        std::cerr << "Failed to read response from server.\n";
        return false;
    }
}