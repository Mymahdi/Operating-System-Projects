CC = g++
CFLAGS = -Wall -O2 -std=c++11 -Iinclude
LDFLAGS = -pthread

BIN_DIR = bin

TARGETS = $(BIN_DIR)/mnist_nn_parallel $(BIN_DIR)/mnist_nn_serial

PARALLEL_DIR = parallel

SERIAL_DIR = serial

SRC_PARALLEL = $(PARALLEL_DIR)/main.cpp \
               $(PARALLEL_DIR)/mnist_utils.cpp \
               $(PARALLEL_DIR)/nn_params.cpp \
               $(PARALLEL_DIR)/display_utils.cpp \
               $(PARALLEL_DIR)/thread_functions.cpp

SRC_SERIAL = $(SERIAL_DIR)/serial.cpp

OBJ_PARALLEL = $(patsubst $(PARALLEL_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRC_PARALLEL))
OBJ_SERIAL = $(BIN_DIR)/serial.o

all: $(TARGETS)

$(BIN_DIR)/mnist_nn_parallel: $(OBJ_PARALLEL)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/mnist_nn_serial: $(OBJ_SERIAL)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(PARALLEL_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/serial.o: $(SERIAL_DIR)/serial.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/mnist_nn_parallel $(BIN_DIR)/mnist_nn_serial

.PHONY: all clean