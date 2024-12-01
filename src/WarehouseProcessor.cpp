#include "WarehouseProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <deque> // For std::deque

using namespace std;

void WarehouseProcessor::processWarehouseFile(const std::string& file_path, std::vector<ProductData>& products) {
    std::ifstream file(file_path);
    std::string line;
    std::map<std::string, std::deque<std::pair<double, int>>> input_transactions;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        //cout << line << endl;
        std::string product_name;
        double price;
        int quantity;
        std::string operation;

        std::getline(ss, product_name, ',');
        ss >> price;
        ss.ignore();
        ss >> quantity;
        ss.ignore();
        ss >> operation;
        //std::getline(ss, operation);
        //cout << operation << endl;

        auto it = std::find_if(products.begin(), products.end(), [&](const ProductData& product) {
            return product.name == product_name;
        });

        if (it != products.end()) {

            if (operation == "output") {
                //cout << price <<" ";
                //cout << quantity << endl;
                // Deduct from the oldest input transactions
                int remaining_quantity = quantity;
                double total_output_profit = 0;
                while (remaining_quantity > 0 && !input_transactions[product_name].empty()) {
                
                    auto& [input_price, input_quantity] = input_transactions[product_name].front();
                    
                    int used_quantity = std::min(remaining_quantity, input_quantity);
                    total_output_profit += used_quantity * (price - input_price);

                    input_quantity -= used_quantity;
                    remaining_quantity -= used_quantity;

                    if (input_quantity == 0) {
                        input_transactions[product_name].pop_front(); // Remove fully used transaction
                    }
                //cout << "profit recieves: ";
                //cout << it->name << total_output_profit << endl;
                }

                // Update product's total remaining stock and profit
                it->total_remaining -= (quantity - remaining_quantity); // Deduct sold quantity
                it->total_profit += total_output_profit;
            } else {
                input_transactions[product_name].emplace_back(price, quantity);
                it->total_remaining += quantity; // Increase stock
            }
        }
    }
       for (const auto& product : products) {
        std::cout << "Product: " << product.name
                  << ", Remaining Stock: " << product.total_remaining
                  << ", Total Profit: " << product.total_profit << std::endl;
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
