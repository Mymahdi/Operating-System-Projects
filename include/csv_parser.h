#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <vector>
#include <string>
#include <map>

// Function to read Parts.csv
std::vector<std::string> readPartsFile(const std::string& filePath);

// Function to parse a warehouse CSV file
std::map<std::string, std::pair<int, int>> parseWarehouseFile(const std::string& filePath);

#endif // CSV_PARSER_H
