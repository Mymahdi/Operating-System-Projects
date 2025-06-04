#include "nn_params.h"
#include "common.h" 
#include <fstream>
#include <sstream>
#include <string>
#include <iostream> 

void allocateHiddenParameters() {
    std::ifstream weights(HIDDEN_WEIGHTS_FILE);
    if (!weights.is_open()) {
        std::cerr << "Error: Could not open hidden weights file: " << HIDDEN_WEIGHTS_FILE << std::endl;
        exit(1);
    }
    for (int idx = 0; idx < NUMBER_OF_HIDDEN_CELLS; idx++) {
        std::string line;
        std::getline(weights, line);
        std::stringstream in(line);
        for (int i = 0; i < NUMBER_OF_INPUT_CELLS; i++) {
            in >> hidden_nodes[idx].weights[i];
        }
    }
    weights.close();

    std::ifstream biases(HIDDEN_BIASES_FILE);
    if (!biases.is_open()) {
        std::cerr << "Error: Could not open hidden biases file: " << HIDDEN_BIASES_FILE << std::endl;
        exit(1);
    }
    for (int idx = 0; idx < NUMBER_OF_HIDDEN_CELLS; idx++) {
        std::string line;
        std::getline(biases, line);
        std::stringstream in(line);
        in >> hidden_nodes[idx].bias;
    }
    biases.close();
}

void allocateOutputParameters() {
    std::ifstream weights(OUTPUT_WEIGHTS_FILE);
    if (!weights.is_open()) {
        std::cerr << "Error: Could not open output weights file: " << OUTPUT_WEIGHTS_FILE << std::endl;
        exit(1);
    }
    for (int idx = 0; idx < NUMBER_OF_OUTPUT_CELLS; idx++) {
        std::string line;
        std::getline(weights, line);
        std::stringstream in(line);
        for (int i = 0; i < NUMBER_OF_HIDDEN_CELLS; i++) {
            in >> output_nodes[idx].weights[i];
        }
    }
    weights.close();

    std::ifstream biases(OUTPUT_BIASES_FILE);
    if (!biases.is_open()) {
        std::cerr << "Error: Could not open output biases file: " << OUTPUT_BIASES_FILE << std::endl;
        exit(1);
    }
    for (int idx = 0; idx < NUMBER_OF_OUTPUT_CELLS; idx++) {
        std::string line;
        std::getline(biases, line);
        std::stringstream in(line);
        in >> output_nodes[idx].bias;
    }
    biases.close();
}