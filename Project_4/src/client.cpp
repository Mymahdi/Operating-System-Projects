
#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <atomic>

#define BUFFER_SIZE 2048

std::atomic<bool> running(true);

void log(const std::string& msg) {
    std::cout << "[CLIENT] " << msg << std::endl;
}

void receiveMessages(int sock) {
    char buffer[BUFFER_SIZE];
    while (running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            log("Disconnected from server.");
            running = false;
            break;
        }

        std::string message(buffer);
        std::cout << "\n[Server]: " << message << "\n> " << std::flush;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./client <username> <port> <role>\n";
        return 1;
    }

    std::string username = argv[1];
    int port = std::stoi(argv[2]);
    std::string role = argv[3];

    if (role != "coder" && role != "navigator") {
        std::cerr << "Role must be 'coder' or 'navigator'.\n";
        return 1;
    }

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

    log("Connected to server.");

    send(sock, username.c_str(), username.length(), 0);
    usleep(100 * 1000);
    send(sock, role.c_str(), role.length(), 0);

    std::thread receiver(receiveMessages, sock);

    std::string input, full_solution;
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (!running) break;

        if (role == "coder") {
            if (input == "/share") {
                send(sock, full_solution.c_str(), full_solution.length(), 0);
                usleep(100 * 1000);
                send(sock, "/share", 6, 0);
                full_solution.clear();  
            } else {
                full_solution += input + "\n";
            }
        } else if (role == "navigator") {
            if (input == "approve" || input == "deny") {
                send(sock, input.c_str(), input.length(), 0);
            }
        }
    }

    if (receiver.joinable()) receiver.join();
    close(sock);
    log("Client exited.");
    return 0;
}


