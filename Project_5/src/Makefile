
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11
BIN_DIR = ../bin

all: $(BIN_DIR)/run_pipeline

$(BIN_DIR)/extract_transform: $(BIN_DIR)/extract_transform.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/load_balance: $(BIN_DIR)/load_balance.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/grid_worker: $(BIN_DIR)/grid_worker.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/final_output: $(BIN_DIR)/final_output.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/%.o: %.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/run_pipeline: $(BIN_DIR)/extract_transform $(BIN_DIR)/load_balance $(BIN_DIR)/grid_worker $(BIN_DIR)/final_output
	echo '#!/bin/bash' > $(BIN_DIR)/run_pipeline
	echo '$(BIN_DIR)/extract_transform "$$@" | $(BIN_DIR)/load_balance | $(BIN_DIR)/final_output' >> $(BIN_DIR)/run_pipeline
	chmod +x $(BIN_DIR)/run_pipeline

clean:
	rm -f $(BIN_DIR)/extract_transform $(BIN_DIR)/load_balance $(BIN_DIR)/grid_worker $(BIN_DIR)/final_output $(BIN_DIR)/run_pipeline $(BIN_DIR)/*.o ../output/GameRanking.csv
