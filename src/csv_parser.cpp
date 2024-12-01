#include "csv_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "logger.h"

std::vector<std::string> readPartsFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        logError("Failed to open Parts file: " + filePath);
        throw std::runtime_error("Failed to open Parts file: " + filePath);
    }

    std::vector<std::string> parts;
    std::string line;
    while (std::getline(file, line, ',')) {
        if (!line.empty()) {
            parts.push_back(line); // Add each product name to the list
        }
    }

    file.close();
    return parts;
}
