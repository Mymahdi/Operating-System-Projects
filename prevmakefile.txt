CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(SRC_DIR)/main.cpp $(SRC_DIR)/ProductData.cpp $(SRC_DIR)/WarehouseProcessor.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

EXEC = warehouse_manager

$(EXEC): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(BUILD_DIR)/$(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(BUILD_DIR)/$(EXEC)
