
#include "../includes/communication_handler.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

CommunicationHandler::CommunicationHandler(int port) : serverPort(port) {}



void CommunicationHandler::startListening() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Error creating socket." << endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Binding failed." << endl;
        close(serverSocket);
        return;
    }

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            cerr << "Failed to accept connection." << endl;
            continue;
        }
        handleClient(clientSocket);
    }
    
    listen(serverSocket, 5);
    cout << "Server listening on port " << serverPort << "..." << endl;
}

