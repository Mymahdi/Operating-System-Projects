#ifndef WAREHOUSEPROCESSOR_H
#define WAREHOUSEPROCESSOR_H

#include <string>
#include <vector>
#include <map>
#include "ProductData.h"

class WarehouseProcessor {
public:
    static void saveProductsToText(const std::vector<ProductData>& products, const std::string& filePath);

    // Function to process warehouse files (CSV format) for a given product
    static void processWarehouseFile(const std::string& file_path, std::vector<ProductData>& products);

    // Function to display product menu to the user
    static void displayProductMenu(const std::vector<ProductData>& products);

    // Function to display profit and stock of a chosen product
    static void displayProfitAndStock(const std::vector<ProductData>& products, int product_choice);

    // Function to save products' data to XML file
    static void saveProductsToXML(const std::vector<ProductData>& products, const std::string& file_path);
};

#endif
