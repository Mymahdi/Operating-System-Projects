# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -lsndfile

# Output binary
OUTPUT = VoiceFilters.out

# Source and header files
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

# Default target: build the program
all: $(OUTPUT)

# Rule to build the output binary
$(OUTPUT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(OBJ) $(LDFLAGS)

# Rule to compile the source file
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)

# Clean up generated files
clean:
	rm -f $(OUTPUT) $(OBJ)

# Run the program (assumes input file path is provided as an argument)
run: $(OUTPUT)
	./$(OUTPUT) ../input.wav

