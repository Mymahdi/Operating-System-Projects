# Pair Programming Challenge Server

A TCP-based server and client system for pair programming challenges where a coder and navigator collaborate to solve programming problems within a time limit.

## Features

- **Role-based interaction**: Supports "coder" and "navigator" roles
- **Problem challenges**: Three predefined programming problems
- **Solution evaluation**: Connects to an evaluation service
- **Real-time communication**: Broadcast messages and private messaging
- **Score calculation**: Time-based scoring with bonuses
- **Session management**: Handles disconnections and time limits

## Components

### Server (`server.cpp`)
- Manages client connections and sessions
- Coordinates problem flow between coder and navigator
- Evaluates solutions through an external service
- Calculates scores based on time and correctness
- Handles time limits and disconnections

### Client (`client.cpp`)
- Connects to server with username and role
- Provides interface for:
  - Coders to write and share solutions
  - Navigators to approve/deny solutions
- Displays server messages in real-time

## Problem Set

1. **Problem 1**: `add_numbers(a, b)` → Return the sum of two integers
2. **Problem 2**: `reverse_string(s)` → Reverse a string
3. **Problem 3**: `is_palindrome(s)` → Check if a string is a palindrome

## Scoring System

- Base points:
  - Problem 1: 1 point
  - Problem 2: 3 points
  - Problem 3: 5 points
- Time bonuses:
  - ≤50% of time limit: +50% bonus
  - ≤75% of time limit: +20% bonus

## Requirements

- C++17 compatible compiler
- Linux/Unix system (uses POSIX sockets)
- Evaluation service running on port 65432

## Building and Running

### Server
```bash
g++ -std=c++17 -pthread server.cpp -o server
./server