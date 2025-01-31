# Rock Paper Scissors Multiplayer Game Report

## Introduction

This project implements a multiplayer Rock-Paper-Scissors game using C++ with socket programming. The system consists of a server and multiple clients, enabling real-time gameplay over a network. Players connect to the server, join game rooms, and compete in rounds of Rock-Paper-Scissors. The game supports multiple rooms, each accommodating two players.

## Objectives

The primary objectives of this project are:
- To demonstrate inter-process communication using sockets.
- To implement a server-client model with multiple concurrent connections.
- To handle real-time user inputs and game state management.
- To explore efficient network communication techniques in C++.

## Project Components

The project consists of two main components:

1. **Server**: Manages client connections, assigns players to rooms, facilitates gameplay, and determines round winners.
2. **Client**: Connects to the server, allows user input, and receives game updates from the server.

---

## Server Code Overview

The server handles multiple clients using `poll()` for managing multiple socket connections. It maintains game rooms, collects player choices, determines winners, and communicates results to clients.

### Features:

- Accepts client connections and assigns them to available rooms.
- Allows players to input their names and select rooms.
- Handles Rock-Paper-Scissors gameplay with a timeout mechanism.
- Uses a separate thread to listen for admin commands (e.g., `end_game`).
- Broadcasts game results and ensures proper client disconnection.

### Server Implementation Details:

- **Socket Setup**: The server creates and binds a TCP socket to listen for incoming client connections.
- **Room Management**: The server dynamically assigns players to rooms and ensures fair matchmaking.
- **Game Loop**: The server waits for both players to make a choice and determines the round winner based on Rock-Paper-Scissors rules.
- **Timeout Handling**: If a player does not respond within 10 seconds, they forfeit the round to ensure smooth gameplay.

### Key Data Structures:

```cpp
struct Room {
    int players[ROOM_CAPACITY] = {-1, -1};
    int player_count = 0;
    map<int, string> playerNames;
    int scores[2] = {0, 0};
    bool gameRunning = true;
};
```

---

## Client Code Overview

The client connects to the server, sends user input, and receives game-related messages.

### Features:

- Connects to the server using TCP sockets.
- Accepts user input for name and room selection.
- Sends Rock-Paper-Scissors choices to the server.
- Receives and displays game results.

### Client Implementation Details:

- **Socket Setup**: The client establishes a connection with the server using TCP sockets.
- **User Interaction**: The client provides prompts for user input and sends responses to the server.
- **Game Participation**: The client sends the user's choice and receives round results from the server.

### Sample Interaction:

```
Enter your name: John
Available rooms:
Room 0 (1/2)
Room 1 (0/2)
Choose a room number: 0
Choose: 0 (Rock), 1 (Paper), 2 (Scissors)
Result: John Wins!
```

---

## System Requirements

To run this project, you need:
- A Linux-based OS (Ubuntu, Debian, etc.) or Windows with WSL.
- A C++ compiler supporting C++11 or later (e.g., g++, clang++).
- Basic networking setup (IP address and port configuration).

## How to Compile and Run the Project

### Compiling the Server and Client

Use the following commands to compile the server and client:

```sh
g++ server.cpp -o server -lpthread
g++ client.cpp -o client
```

### Running the Server:

```sh
./server <IP> <PORT> <NUMBER_OF_ROOMS>
```

Example:

```sh
./server 127.0.0.1 8080 5
```

### Running the Client:

```sh
./client <IP> <PORT>
```

Example:

```sh
./client 127.0.0.1 8080
```

---

## Challenges Faced

During the development of this project, we encountered and overcame the following challenges:
- **Handling multiple clients**: Using `poll()` effectively to manage simultaneous client connections.
- **Synchronization issues**: Ensuring both players in a room submit their choices before processing results.
- **Timeout handling**: Implementing a mechanism to handle cases where a player does not respond.
- **Ensuring fair matchmaking**: Assigning players to rooms dynamically while avoiding conflicts.

## Conclusion

This project demonstrates a simple yet effective multiplayer Rock-Paper-Scissors game using C++ sockets. The server efficiently manages multiple clients and game sessions, ensuring fair play through a structured timeout mechanism. The client provides an interactive way for players to connect and participate in the game.

### Possible Enhancements:

- Implement a GUI for better user experience.
- Improve error handling and reconnection mechanisms.
- Add a leaderboard system for tracking player scores.
- Support more than two players per game room.
- Implement a ranking system for players.

---

**Language Used:** C++  
**Operating System:** Linux  
