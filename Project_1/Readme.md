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
