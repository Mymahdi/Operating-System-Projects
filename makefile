# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Source files
SRC_MAIN = src/main.cpp
SRC_MAPPER = src/MapperProcess.cpp
SRC_REDUCER = src/ReducerProcess.cpp

# Output binaries
BIN_MAIN = main
BIN_MAPPER = MapperProcess
BIN_REDUCER = ReducerProcess

# Default target
all: $(BIN_MAIN) $(BIN_MAPPER) $(BIN_REDUCER)

# Compile main.cpp
$(BIN_MAIN): $(SRC_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile MapperProcess.cpp
$(BIN_MAPPER): $(SRC_MAPPER)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile ReducerProcess.cpp
$(BIN_REDUCER): $(SRC_REDUCER)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up generated files
clean:
	rm -f $(BIN_MAIN) $(BIN_MAPPER) $(BIN_REDUCER)

# Phony targets
.PHONY: all clean
