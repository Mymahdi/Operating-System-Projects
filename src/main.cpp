#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem> // For std::filesystem
#include "ProductData.h"
#include "WarehouseProcessor.h"

using namespace std;
namespace fs = std::filesystem;

int main() {
    // Read the Parts.csv file to get a list of product names
    ifstream parts_file("stores/Parts.csv");
    string line;
    vector<string> product_names;
    while (getline(parts_file, line)) {
        stringstream ss(line);
        string product;
        while (getline(ss, product, ',')) {
            product_names.push_back(product);
        }
    }

    // Create ProductData objects for each product in Parts.csv
    vector<ProductData> products;
    for (const string& product_name : product_names) {
        products.emplace_back(product_name, 0);  // Profit starts at 0 for all products
    }

    // Process all CSV files dynamically detected in the 'stores' folder
    for (const auto& entry : fs::directory_iterator("stores")) {
        if (entry.path().extension() == ".csv" && entry.path().filename() != "Parts.csv") {
            cout << "reading file:" + entry.path().string() << endl;
            
            WarehouseProcessor::processWarehouseFile(entry.path().string(), products);
        }
    }

    // Display product list and prompt user to choose a product
    WarehouseProcessor::displayProductMenu(products);
    int product_choice;
    cout << "Enter the number of the product you want to view: ";
    cin >> product_choice;

    // Display profit and remaining stock of the chosen product
    WarehouseProcessor::displayProfitAndStock(products, product_choice);

    return 0;
}
