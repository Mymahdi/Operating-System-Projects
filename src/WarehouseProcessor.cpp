#include "WarehouseProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <deque>
#include <tinyxml2.h>

using namespace std;
using namespace tinyxml2;

// Function to process a warehouse file (CSV) and update the product list
void WarehouseProcessor::processWarehouseFile(const std::string& file_path, std::vector<ProductData>& products) {
    std::ifstream file(file_path);
    std::string line;
    std::map<std::string, std::deque<std::pair<double, int>>> input_transactions;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
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

        auto it = std::find_if(products.begin(), products.end(), [&](const ProductData& product) {
            return product.name == product_name;
        });

        if (it != products.end()) {
            if (operation == "output") {
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
                }

                it->total_remaining -= (quantity - remaining_quantity);
                it->total_profit += total_output_profit;
            } else {
                input_transactions[product_name].emplace_back(price, quantity);
                it->total_remaining += quantity;
            }
        }
    }

    for (const auto& product : products) {
        std::cout << "Product: " << product.name
                  << ", Remaining Stock: " << product.total_remaining
                  << ", Total Profit: " << product.total_profit << std::endl;
    }
}

// Function to display a menu of products for the user to select
void WarehouseProcessor::displayProductMenu(const std::vector<ProductData>& products) {
    std::cout << "Select a product to view profit and remaining stock:" << std::endl;
    int index = 1;
    for (const auto& product : products) {
        std::cout << index++ << ". " << product.name << std::endl;
    }
}

// Function to display profit and stock for the selected product
void WarehouseProcessor::displayProfitAndStock(const std::vector<ProductData>& products, int product_choice) {
    if (static_cast<size_t>(product_choice) >= 1 && static_cast<size_t>(product_choice) <= products.size()) {
        products[product_choice - 1].printProductInfo();
    } else {
        std::cout << "Invalid product choice!" << std::endl;
    }
}

// Function to save product data to an XML file

void WarehouseProcessor::saveProductsToText(const std::vector<ProductData>& products, const std::string& filePath) {
    std::ofstream outFile(filePath);

    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing: " << filePath << std::endl;
        return;
    }

    for (const auto& product : products) {
        outFile << "Product: " << product.name
                << ", Total Profit: " << product.total_profit
                << ", Remaining Stock: " << product.total_remaining
                << std::endl;
    }

    outFile.close();
    std::cout << "Product data saved to " << filePath << std::endl;
}



// void WarehouseProcessor::saveProductsToXML(const std::vector<ProductData>& products, const std::string& file_path) {
//     XMLDocument doc;

//     // Create XML declaration
//     XMLNode* decl = doc.NewDeclaration();
//     doc.InsertFirstChild(decl);

//     // Create root element <Products>
//     XMLElement* root = doc.NewElement("Products");
//     doc.InsertEndChild(root);

//     // Add product elements
//     for (const auto& product : products) {
//         XMLElement* productElement = doc.NewElement("Product");

//         XMLElement* name = doc.NewElement("Name");
//         name->SetText(product.name.c_str());
//         productElement->InsertEndChild(name);

//         XMLElement* profit = doc.NewElement("TotalProfit");
//         profit->SetText(product.total_profit);
//         productElement->InsertEndChild(profit);

//         XMLElement* stock = doc.NewElement("TotalRemaining");
//         stock->SetText(product.total_remaining);
//         productElement->InsertEndChild(stock);

//         root->InsertEndChild(productElement);
//     }

//     // Save the XML file
//     doc.SaveFile(file_path.c_str());
// }
