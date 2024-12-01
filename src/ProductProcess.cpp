#include <iostream>
#include <fstream>
#include <sstream>
#include "ProductData.h"

// Function to aggregate product data from FIFO (or pipe)
void aggregateProductData(const std::string& fifoPath, const std::string& productName);

// Function to simulate aggregation
void processProduct(const std::string& productName) {
    std::cout << "Processing Product: " << productName << "\n";
    // Example logic to process a product
}

void aggregateProductData(const std::string& fifoPath, const std::string& productName) {
    // Simulate reading from FIFO and aggregating
    processProduct(productName);
}
