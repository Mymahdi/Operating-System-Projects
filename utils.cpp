#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

std::string extractProductKey(const std::string& data) {
    std::istringstream iss(data);
    std::string key;
    std::getline(iss, key, ','); // Assuming key is the first field
    return key;
}

// Function to parse the parts.csv file to get the list of product names
std::vector<std::string> parsePartsFile(const std::string& partsFilePath) {
    std::ifstream file(partsFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open parts file: " << partsFilePath << std::endl;
        return {};
    }

    std::vector<std::string> products;
    std::string line;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string product;
        while (std::getline(ss, product, ',')) {
            products.push_back(product);
        }
    }

    file.close();
    return products;
}

// Function to list files in a directory, excluding a specific file
std::vector<std::string> listFiles(const std::string& directory, const std::string& excludeFile) {
    std::vector<std::string> files;

    for (const auto& entry : fs::directory_iterator(directory)) {
        std::string filePath = entry.path().string();
        if (filePath.find(excludeFile) == std::string::npos) {
            files.push_back(filePath);
        }
    }

    return files;
}

// Function to parse a warehouse file and extract inventory data
std::vector<InventoryData> parseWarehouseFile(const std::string& warehouseFilePath) {
    std::ifstream file(warehouseFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open warehouse file: " << warehouseFilePath << std::endl;
        return {};
    }

    std::vector<InventoryData> inventory;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        InventoryData data;
        std::getline(ss, data.productName, ',');
        ss >> data.quantity;
        ss.ignore(); // Ignore the comma
        ss >> data.pricePerUnit;
        inventory.push_back(data);
    }

    file.close();
    return inventory;
}

// Function to calculate remaining stock and profits for a warehouse
std::vector<ProductData> calculateWarehouseData(const std::vector<InventoryData>& inventory) {
    std::vector<ProductData> results;

    for (const auto& item : inventory) {
        ProductData data;
        data.productName = item.productName;
        data.remainingStock = item.quantity;
        data.totalProfit = item.quantity * item.pricePerUnit;
        results.push_back(data);
    }

    return results;
}

// Function to serialize product data into a string for transmission
std::string serializeData(const std::vector<ProductData>& data) {
    std::stringstream ss;

    for (const auto& item : data) {
        ss << item.productName << "," << item.remainingStock << "," << item.totalProfit << "\n";
    }

    return ss.str();
}

// Function to deserialize a string back into product data
std::vector<ProductData> deserializeData(const std::string& serializedData) {
    std::vector<ProductData> data;
    std::stringstream ss(serializedData);
    std::string line;

    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        ProductData item;
        std::getline(lineStream, item.productName, ',');
        lineStream >> item.remainingStock;
        lineStream.ignore(); // Ignore the comma
        lineStream >> item.totalProfit;
        data.push_back(item);
    }

    return data;
}

// Function to aggregate data for a product across multiple warehouses
ProductSummary aggregateProductData(const std::vector<ProductData>& productData) {
    ProductSummary summary;
    summary.totalStock = 0;
    summary.totalProfit = 0.0;

    for (const auto& item : productData) {
        summary.productName = item.productName; // Assumes all data is for the same product
        summary.totalStock += item.remainingStock;
        summary.totalProfit += item.totalProfit;
    }

    return summary;
}
