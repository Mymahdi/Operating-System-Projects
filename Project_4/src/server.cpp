#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <poll.h>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIME_LIMIT 60
#define EVALUATION_PORT 65432 

using namespace std;

std::mutex mtx;

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket.\n";
        return 1;
    }

    sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) {
        std::cerr << "Bind failed.\n";
        return 1;
    }

    if (listen(server_socket, 2) < 0) {
        std::cerr << "Listen failed.\n";
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr {};
        socklen_t client_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);
        if (client_socket >= 0) {
            std::cout << "[LOG] New client connected." << std::endl;
            std::thread(handle_client, client_socket).detach();
        }
    }

    close(server_socket);
    return 0;
}