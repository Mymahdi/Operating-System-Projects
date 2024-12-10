#include "ProductData.h"
#include <iostream>

ProductData::ProductData(const std::string& name, double price, int quantity)
    : productName(name), price(price), quantity(quantity), totalProfit(0) {}

void ProductData::addTransaction(int qty, double profit) {
    quantity += qty;
    totalProfit += profit;
}

void ProductData::printProductInfo() const {
    std::cout << "Product: " << productName << "\n";
    std::cout << "Price: " << price << "\n";
    std::cout << "Quantity: " << quantity << "\n";
    std::cout << "Total Profit: " << totalProfit << "\n";
}
