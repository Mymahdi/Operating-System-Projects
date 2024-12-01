#ifndef WAREHOUSEPROCESSOR_H
#define WAREHOUSEPROCESSOR_H

#include <string>
#include <vector>
#include <map>
#include "ProductData.h"

class WarehouseProcessor {
public:
    static void saveProductsToText(const std::vector<ProductData>& products, const std::string& filePath);

    static void processWarehouseFile(const std::string& file_path, std::vector<ProductData>& products);

    static void displayProductMenu(const std::vector<ProductData>& products);

    static void displayProfitAndStock(const std::vector<ProductData>& products, int product_choice);

    static void saveProductsToXML(const std::vector<ProductData>& products, const std::string& file_path);
};

#endif
