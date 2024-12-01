#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <map>

// Function to read the Parts.csv file
std::vector<std::string> readPartsFile(const std::string& filePath);

// Function to parse a warehouse .csv file (input/output)
std::map<std::string, std::pair<int, int>> parseWarehouseFile(const std::string& filePath);

#endif // UTILS_H
