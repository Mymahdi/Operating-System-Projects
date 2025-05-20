CC = g++
CFLAGS = -Wall -O2 -std=c++11
LDFLAGS = -pthread

TARGETS = parallel serial

SRC_PARALLEL = parallel.cpp
SRC_SERIAL = serial.cpp

OBJ_PARALLEL = $(SRC_PARALLEL:.cpp=.o)
OBJ_SERIAL = $(SRC_SERIAL:.cpp=.o)

all: $(TARGETS)

parallel: $(OBJ_PARALLEL)
	$(CC) $< -o $@ $(LDFLAGS)

serial: $(OBJ_SERIAL)
	$(CC) $< -o $@

$(OBJ_PARALLEL): $(SRC_PARALLEL)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_SERIAL): $(SRC_SERIAL)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_PARALLEL) $(OBJ_SERIAL) $(TARGETS)

.PHONY: all clean