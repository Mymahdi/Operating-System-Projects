#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

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

// Function to test parsing Parts.csv and warehouse files in the stores directory
void testParsing() {
    std::string partsFilePath = "stores/Parts.csv";
    std::vector<std::string> products = readPartsFile(partsFilePath);
    
    std::cout << "Products in Parts.csv:" << std::endl;
    for (const auto& product : products) {
        std::cout << product << std::endl;
    }

    // Read all warehouse files in the stores directory
    std::cout << "\nParsing warehouse data from 'stores/' directory:" << std::endl;
    for (const auto& entry : fs::directory_iterator("stores/")) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv" && entry.path().filename() != "Parts.csv") {
            std::cout << "\nProcessing file: " << entry.path().filename() << std::endl;
            auto data = parseWarehouseFile(entry.path().string());
            
            for (const auto& [product, stats] : data) {
                std::cout << "Product: " << product << ", Quantity: " << stats.first << ", Profit: " << stats.second << std::endl;
            }
        }
    }
}

int main() {
    testParsing();
    return 0;
}
