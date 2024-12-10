#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./client <IP> <PORT>\n";
        return 1;
    }

    const char* ip = argv[1];
    int port = stoi(argv[2]);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection to server failed");
        close(clientSocket);
        return 1;
    }

    char buffer[1024] = {0};

    read(clientSocket, buffer, 1024);
    cout << buffer;

    string playerName;
    getline(cin, playerName);
    send(clientSocket, playerName.c_str(), playerName.size() + 1, 0);

    while (true) {
        int bytesRead = read(clientSocket, buffer, 1024);
        if (bytesRead <= 0) break;

        cout << string(buffer, bytesRead);

        string message(buffer, bytesRead);
        if (message.find("Choose") != string::npos) {
            string playerMove;
            getline(cin, playerMove);

            send(clientSocket, playerMove.c_str(), playerMove.size() + 1, 0);

            if (playerMove == "quit") {
                cout << "You have exited the game.\n";
                break;
            }
        }
    }

    close(clientSocket);
    return 0;
}

