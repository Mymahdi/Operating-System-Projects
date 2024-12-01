#ifndef PRODUCTDATA_H
#define PRODUCTDATA_H

#include <string>
#include <vector>

class ProductData {
public:
    std::string productName;
    double price;
    int quantity;
    double totalProfit;
    int remainingStock;

    ProductData(const std::string& name, double price, int quantity);

    void addTransaction(int qtySold, double salePrice);
    void printProductInfo() const;
};

#endif
