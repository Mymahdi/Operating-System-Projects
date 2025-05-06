CC = g++
CFLAGS = -Wall -Wextra -std=c++11
SRCDIR = src
TARGETS = server client

all: $(TARGETS)

server: $(SRCDIR)/server.cpp
	$(CC) $(CFLAGS) -o server $(SRCDIR)/server.cpp

client: $(SRCDIR)/client.cpp
	$(CC) $(CFLAGS) -o client $(SRCDIR)/client.cpp

clean:
	rm -f $(TARGETS)
