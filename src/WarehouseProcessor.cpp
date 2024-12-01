#include "WarehouseProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

void WarehouseProcessor::processWarehouseFile(const std::string& file_path, std::vector<ProductData>& products) {
    std::ifstream file(file_path);
    std::string line;
    std::map<std::string, double> latest_input_prices; // To store the latest input price for each product

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string product_name;
        double price;
        int quantity;
        std::string operation;

        std::getline(ss, product_name, ',');
        ss >> price;
        ss.ignore();  // Ignore the comma
        ss >> quantity;
        ss.ignore();  // Ignore the comma
        std::getline(ss, operation);

        auto it = std::find_if(products.begin(), products.end(), [&](const ProductData& product) {
            return product.name == product_name;
        });

        if (it != products.end()) {
            // Calculate profit based on the operation (input or output)
            if (operation == "input") {
                latest_input_prices[product_name] = price;
                it->total_remaining += quantity;  // Increase the stock for input operations
            } else if (operation == "output") {
                it->addTransaction(price, quantity, latest_input_prices[product_name]);
            }
        }
    }
}

void WarehouseProcessor::displayProductMenu(const std::vector<ProductData>& products) {
    std::cout << "Select a product to view profit and remaining stock:" << std::endl;
    int index = 1;
    for (const auto& product : products) {
        std::cout << index++ << ". " << product.name << std::endl;
    }
}

void WarehouseProcessor::displayProfitAndStock(const std::vector<ProductData>& products, int product_choice) {
    if (static_cast<size_t>(product_choice) >= 1 && static_cast<size_t>(product_choice) <= products.size()) {
        products[product_choice - 1].printProductInfo();
    } else {
        std::cout << "Invalid product choice!" << std::endl;
    }
}
