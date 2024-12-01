#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ProductData.h"
#include "WarehouseProcessor.h"

using namespace std;

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
        products.emplace_back(product_name, 0);  // Price will be updated dynamically during warehouse file processing
    }

    // Process all dynamic warehouse CSV files (assuming they are in the 'stores' folder)
    for (const string& warehouse_file : {"stores/warehouse_1.csv", "stores/warehouse_2.csv"}) { // Add more warehouse files as needed
        WarehouseProcessor::processWarehouseFile(warehouse_file, products);
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
