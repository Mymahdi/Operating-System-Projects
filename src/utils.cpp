#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "utils.h"

// Function to read the Parts.csv file
std::vector<std::string> readPartsFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> parts;
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return parts;
    }

    std::string line;
    while (std::getline(file, line, ',')) {
        parts.push_back(line);
    }

    file.close();
    return parts;
}

// Function to parse a warehouse .csv file (input/output)
std::map<std::string, std::pair<int, int>> parseWarehouseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::map<std::string, std::pair<int, int>> data; // {product: {quantity, profit}}

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string product, type;
        int quantity, price;

        std::getline(ss, product, ','); // Read product name
        ss >> quantity;                  // Read quantity
        ss.ignore(1, ',');               // Skip comma
        ss >> price;                     // Read price
        ss.ignore(1, ',');               // Skip comma
        std::getline(ss, type);          // Read type (input/output)

        if (type == "input") {
            data[product].first += quantity; // Add to stock
        } else if (type == "output") {
            data[product].first -= quantity; // Subtract from stock
            data[product].second += quantity * price; // Calculate profit
        }
    }

    file.close();
    return data;
}
