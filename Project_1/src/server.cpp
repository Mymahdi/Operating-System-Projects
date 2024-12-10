#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <thread>
#include <map>
#include <chrono>

using namespace std;

#define TIMEOUT 10000
#define MAX_CLIENTS 10
#define ROOM_CAPACITY 2

enum Choice { ROCK, PAPER, SCISSORS, NONE };

string choiceToString(Choice choice) {
    switch(choice) {
        case ROCK: return "Rock";
        case PAPER: return "Paper";
        case SCISSORS: return "Scissors";
        default: return "None";
    }
}

string determineRoundWinner(Choice c1, Choice c2) {
    if (c1 == c2) return "Draw";
    if ((c1 == ROCK && c2 == SCISSORS) || (c1 == PAPER && c2 == ROCK) || (c1 == SCISSORS && c2 == PAPER)) return "Player 1 Wins";
    return "Player 2 Wins";
}

struct Room {
    int players[ROOM_CAPACITY] = {-1, -1};
    int player_count = 0;
    map<int, string> playerNames;
    int scores[2] = {0, 0};
    bool gameRunning = true;
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <IP> <PORT> <NUMBER_OF_ROOMS>\n";
        return -1;
    }

    int server_fd, new_socket, numRooms = stoi(argv[3]);
    sockaddr_in address;
    int addrlen = sizeof(address);
    vector<int> clients;
    vector<Room> rooms(numRooms);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(stoi(argv[2]));

    if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server is listening on IP " << argv[1] << " and port " << argv[2] << "\n";

    pollfd fds[MAX_CLIENTS + 1];
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    int client_count = 0;
    bool serverRunning = true;

    thread([&]() {
        string command;
        while (true) {
            getline(cin, command);
            if (command == "end_game") {
                serverRunning = false;

                for (int i = 0; i < numRooms; i++) {
                    Room& room = rooms[i];
                    if (room.player_count == ROOM_CAPACITY) {
                        string matchWinner;
                        if (room.scores[0] > room.scores[1]) {
                            matchWinner = room.playerNames[room.players[0]] + " is the overall match winner!\n";
                        } else if (room.scores[1] > room.scores[0]) {
                            matchWinner = room.playerNames[room.players[1]] + " is the overall match winner!\n";
                        } else {
                            matchWinner = "The match is a Draw!\n";
                        }

                        for (int player_fd : room.players) {
                            send(player_fd, matchWinner.c_str(), matchWinner.size(), 0);
                            close(player_fd);
                        }
                        room.player_count = 0;
                        room.gameRunning = false;
                        cout << "Match result for Room " << i << ": " << matchWinner << "\n";
                    }
                }

                cout << "Game has been terminated by the organizer.\n";
                break;
            }
        }
    }).detach();

    while (serverRunning) {
        int poll_count = poll(fds, client_count + 1, TIMEOUT);

        if (poll_count == -1) {
            perror("Poll error");
            break;
        }

        if (fds[0].revents & POLLIN && client_count < MAX_CLIENTS) {
            if ((new_socket = accept(server_fd, (sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("Accept");
                exit(EXIT_FAILURE);
            }

            cout << "New client connected.\n";
            fds[client_count + 1].fd = new_socket;
            fds[client_count + 1].events = POLLIN;
            clients.push_back(new_socket);
            client_count++;

            string namePrompt = "Enter your name: ";
            send(new_socket, namePrompt.c_str(), namePrompt.size(), 0);

            char buffer[1024] = {0};
            read(new_socket, buffer, 1024);
            string playerName = buffer;

            string roomPrompt = "Available rooms:\n";
            for (int i = 0; i < numRooms; i++) {
                roomPrompt += "Room " + to_string(i) + " ("
                    + to_string(rooms[i].player_count) + "/" + to_string(ROOM_CAPACITY) + ")\n";
            }
            roomPrompt += "Choose a room number: ";
            send(new_socket, roomPrompt.c_str(), roomPrompt.size(), 0);

            memset(buffer, 0, 1024);
            read(new_socket, buffer, 1024);
            int roomChoice = stoi(buffer);

            Room& room = rooms[roomChoice];
            if (room.player_count < ROOM_CAPACITY) {
                room.players[room.player_count] = new_socket;
                room.playerNames[new_socket] = playerName;
                room.player_count++;
            }

            if (room.player_count == ROOM_CAPACITY) {
                cout << "Starting game in room " << roomChoice << "...\n";

                while (room.gameRunning) {
                    Choice choices[2] = {NONE, NONE};
                    bool playerTimedOut[2] = {true, true};

                    auto start_time = chrono::steady_clock::now();
                    for (int i = 0; i < ROOM_CAPACITY; i++) {
                        string menu = "Choose: 0 (Rock), 1 (Paper), 2 (Scissors)\n";
                        send(room.players[i], menu.c_str(), menu.size(), 0);
                    }

                    while (chrono::duration_cast<chrono::milliseconds>(
                               chrono::steady_clock::now() - start_time)
                           .count() < TIMEOUT) {
                        for (int i = 0; i < ROOM_CAPACITY; i++) {
                            if (choices[i] == NONE) {
                                int result = poll(&fds[i + 1], 1, TIMEOUT);
                                if (result > 0 && (fds[i + 1].revents & POLLIN)) {
                                    memset(buffer, 0, 1024);
                                    read(room.players[i], buffer, 1024);
                                    int choice = stoi(buffer);
                                    if (choice >= 0 && choice <= 2) {
                                        choices[i] = static_cast<Choice>(choice);
                                        playerTimedOut[i] = false;
                                    }
                                }
                            }
                        }
                    }

                    string result;
                    if (playerTimedOut[0] && playerTimedOut[1]) {
                        result = "Draw (both players timed out)";
                    } else if (playerTimedOut[0]) {
                        result = room.playerNames[room.players[1]] + " Wins (other player timed out)";
                        room.scores[1]++;
                    } else if (playerTimedOut[1]) {
                        result = room.playerNames[room.players[0]] + " Wins (other player timed out)";
                        room.scores[0]++;
                    } else {
                        result = determineRoundWinner(choices[0], choices[1]);
                        if (result == "Player 1 Wins") {
                            result = room.playerNames[room.players[0]] + " Wins!";
                            room.scores[0]++;
                        } else if (result == "Player 2 Wins") {
                            result = room.playerNames[room.players[1]] + " Wins!";
                            room.scores[1]++;
                        }
                    }

                    result = "Result: " + result + "\n";
                    for (int i = 0; i < ROOM_CAPACITY; i++) {
                        send(room.players[i], result.c_str(), result.size(), 0);
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}

