# Project_1
# Rock-Paper-Scissors Multiplayer Game

## Project Overview

This project is a **multiplayer Rock-Paper-Scissors game** built using **C++ and socket programming**. It consists of a **server** that manages game rooms and clients that connect to play against each other. The game allows real-time interaction between players over a network, handling multiple game rooms simultaneously.

### Key Features:
- **Server-Client Model:** A centralized server manages player connections and gameplay.
- **Multiple Game Rooms:** Players can join different rooms and compete in rounds.
- **Real-Time Gameplay:** The game processes player inputs and determines winners instantly.
- **Timeout Mechanism:** Players must make a move within 10 seconds, or they forfeit the round.
- **Admin Commands:** The server supports commands like `end_game` to manage the game state.

---

## Project Objectives

The main goals of this project are:
1. **Implement Socket Programming:** Enable communication between multiple clients and a server using TCP sockets.
2. **Manage Multiple Clients:** Use efficient techniques (e.g., `poll()`) to handle multiple players concurrently.
3. **Develop a Real-Time Game System:** Process user inputs, determine game results, and ensure smooth interactions.
4. **Improve Networking Skills in C++:** Explore networking concepts such as connection handling, data transmission, and concurrency.
5. **Enhance System Robustness:** Implement timeouts, error handling, and fair matchmaking for a stable gaming experience.

---

</br>
</br>

# Project_2 
# Warehouse Data Processing System

## Overview

This project is a distributed system that processes warehouse transaction data using the **Map-Reduce** paradigm. It reads CSV files containing warehouse transactions, processes them using multiple **mapper processes**, and aggregates the results using a **reducer process**. The system is implemented in **C++** and utilizes **Unix system calls** for process management and inter-process communication (IPC).

## Objectives

- Implement a distributed warehouse data processing system.
- Utilize **Map-Reduce** for parallel processing.
- Use **multiple mapper processes** to read warehouse CSV files.
- Implement a **reducer process** to aggregate product transactions.
- Manage inter-process communication (IPC) via **pipes**.
- Log process execution details for monitoring and debugging.

## Workflow

1. **Data Collection**: Gather warehouse transaction CSV files.
2. **Mapper Phase**: Process each CSV file in parallel.
3. **Reducer Phase**: Aggregate data and compute totals.
4. **Logging**: Record processing details in a log file.

This system efficiently handles large datasets and provides a scalable solution for warehouse data analysis.

