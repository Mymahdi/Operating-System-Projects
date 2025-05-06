#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <atomic>

#define BUFFER_SIZE 2048
std::atomic<bool> running(true);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./client <username> <port> <role>\n";
        return 1;
    }

    std::string username = argv[1];
    int port = std::stoi(argv[2]);
    std::string role = argv[3];

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // ... rest of main function ...
}