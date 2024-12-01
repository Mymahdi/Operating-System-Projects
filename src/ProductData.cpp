#include "ProductData.h"
#include <iostream>

void ProductData::addTransaction(double output_price, int output_quantity, double input_price) {
    if (true) {
        total_profit += (output_price - input_price) * output_quantity;
        quantity -= output_quantity;
    } 
}

void ProductData::printProductInfo() const {
    std::cout << "Product: " << name << std::endl;
    std::cout << "Total Profit: " << total_profit << std::endl;
    std::cout << "Remaining Stock: " << total_remaining << std::endl;
}
