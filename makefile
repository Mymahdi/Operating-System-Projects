# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Directories
SRC_DIR = src
BIN_DIR = bin
STORES_DIR = stores

# Target executables
MAIN = $(BIN_DIR)/main
MAPPER = $(BIN_DIR)/MapperProcess
REDUCER = $(BIN_DIR)/ReducerProcess

# Source files
MAIN_SRC = $(SRC_DIR)/main.cpp
MAPPER_SRC = $(SRC_DIR)/MapperProcess.cpp
REDUCER_SRC = $(SRC_DIR)/ReducerProcess.cpp

# Default target
all: setup $(MAIN) $(MAPPER) $(REDUCER)

# Create binary directory if it doesn't exist
setup:
	mkdir -p $(BIN_DIR)

# Compile main program
$(MAIN): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Compile Mapper process
$(MAPPER): $(MAPPER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Compile Reducer process
$(REDUCER): $(REDUCER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Clean target to remove build artifacts
clean:
	rm -rf $(BIN_DIR)

# Run target to execute the main program
run: all
	./$(MAIN)
