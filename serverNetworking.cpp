// Zachary Schupbach
// 11/26/25
// This code does the server side network connection waiting for user connection
// Gets string from user and returns a string
#include "serverNetworking.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

const int PORT = 8080;
const std::string SECRET = "SECRET_KEY";

// Define the core application logic function here or link it from elsewhere
// This function gets the clean input and must return the response string
std::string processClientRequest(const std::string& requestData) {
    
}


void handleClientConnection(int clientSocket) {
    char buffer[1024] = {0};
    long valread = read(clientSocket, buffer, sizeof(buffer) - 1);

    if (valread > 0) {
        std::string receivedData(buffer);

        if (receivedData.substr(0, 10) == SECRET) {
            std::string actualData = receivedData.substr(10);
            std::cout << "Processing request: " << actualData << std::endl;
            
            // Call the business logic function to get the response string
            std::string response = processClientRequest(actualData);
            
            // Send the response back to the client
            send(clientSocket, response.c_str(), response.length(), 0);

        } else {
            std::cout << "Unauthorized connection attempt blocked." << std::endl;
            send(clientSocket, "Unauthorized", 12, 0);
        }
    }
    close(clientSocket);
}

void startServerListener() {
    int serverFd, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
      
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(serverFd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listener started on port " << PORT << ". Waiting for connections...\n";

    // This loop keeps the server running indefinitely
    while(true) {
        if ((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        handleClientConnection(newSocket);
        std::cout << "Connection handled, waiting for next connection...\n";
    }
    // Code below is unreachable in this simple setup
    // close(serverFd);
}