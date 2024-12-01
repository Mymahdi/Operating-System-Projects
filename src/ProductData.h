#ifndef PRODUCTDATA_H
#define PRODUCTDATA_H

#include <string>

class ProductData {
public:
    std::string name;
    double price;          // price per unit
    int quantity;          // quantity of product
    double total_profit;   // total profit from all warehouses
    int total_remaining;   // total remaining stock from all warehouses

    ProductData(const std::string& product_name, double product_price)
        : name(product_name), price(product_price), total_profit(0), total_remaining(0) {}

    void addTransaction(double output_price, int output_quantity, double input_price);
    void printProductInfo() const;
};

#endif
