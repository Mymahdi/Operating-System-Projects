#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>


// Structure to store inventory data for warehouses
struct InventoryData {
    std::string productName;
    int quantity;
    double pricePerUnit;
};

// Structure to store processed data for a single product
struct ProductData {
    std::string productName;
    int remainingStock;
    double totalProfit;
};

// Structure to summarize data for a product across all warehouses
struct ProductSummary {
    std::string productName;
    int totalStock;
    double totalProfit;
};

// Function declarations
std::string extractProductKey(const std::string& data);
std::vector<std::string> parsePartsFile(const std::string& partsFilePath);
std::vector<std::string> listFiles(const std::string& directory, const std::string& excludeFile);
std::vector<InventoryData> parseWarehouseFile(const std::string& warehouseFilePath);
std::vector<ProductData> calculateWarehouseData(const std::vector<InventoryData>& inventory);
std::string serializeData(const std::vector<ProductData>& data);
std::vector<ProductData> deserializeData(const std::string& serializedData);
ProductSummary aggregateProductData(const std::vector<ProductData>& productData);

#endif
