#include <iostream>
#include <fstream>
#include <sstream>
#include "ProductData.h"

void aggregateProductData(const std::string& fifoPath, const std::string& productName);

void processProduct(const std::string& productName) {
    std::cout << "Processing Product: " << productName << "\n";
}

void aggregateProductData(const std::string& fifoPath, const std::string& productName) {
    processProduct(productName);
}
