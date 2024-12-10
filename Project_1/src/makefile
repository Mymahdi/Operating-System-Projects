CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lpthread  # Link pthread library for threading

# Targets
all: server.out client.out

# Server executable
server.out: ./server.cpp
	$(CXX) $(CXXFLAGS) -o server.out ./server.cpp $(LDFLAGS)

# Client executable
client.out: ./client.cpp
	$(CXX) $(CXXFLAGS) -o client.out ./client.cpp

# Clean up
clean:
	rm -f server.out client.out
