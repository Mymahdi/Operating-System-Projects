#include "ReducerAggregator.h"
#include <iostream>
#include <sstream>
#include <string>


void ReducerAggregator::aggregateDataFromPipe() {

    std::string line;
    while (std::getline(std::cin, line)) {
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

        // Aggregate data for each product (simplified example)
        std::cout << "Aggregating data for: " << product_name << std::endl;
        // Perform actual aggregation logic here
    }
}
