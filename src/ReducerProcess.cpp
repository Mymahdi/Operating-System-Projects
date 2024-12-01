#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <deque>

struct ProductData {
    double totalProfit = 0;
    int totalRemaining = 0;
    std::deque<std::pair<double, int>> inputTransactions;
};

int main() {
    std::map<std::string, ProductData> productMap;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream ss(line);
        std::string productName, operation;
        double price;
        int quantity;

        std::getline(ss, productName, ',');
        ss >> price;
        ss.ignore();
        ss >> quantity;
        ss.ignore();
        ss >> operation;

        auto& product = productMap[productName];
        if (operation == "input") {
            product.inputTransactions.emplace_back(price, quantity);
            product.totalRemaining += quantity;
        } else if (operation == "output") {
            int remainingQuantity = quantity;
            while (remainingQuantity > 0 && !product.inputTransactions.empty()) {
                auto& [inputPrice, inputQuantity] = product.inputTransactions.front();
                int usedQuantity = std::min(remainingQuantity, inputQuantity);
                product.totalProfit += usedQuantity * (price - inputPrice);

                inputQuantity -= usedQuantity;
                remainingQuantity -= usedQuantity;

                if (inputQuantity == 0) {
                    product.inputTransactions.pop_front();
                }
            }
            product.totalRemaining -= (quantity - remainingQuantity);
        }
    }

    // Output consolidated results
    for (const auto& [productName, product] : productMap) {
        std::cout << "Product: " << productName
                  << ", Total Profit: " << product.totalProfit
                  << ", Remaining Stock: " << product.totalRemaining << std::endl;
    }

    return 0;
}
