#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <deque>
#include <unistd.h>

using namespace std;

struct ProductData {
    double totalProfit = 0;
    int totalRemaining = 0;
    deque<pair<double, int>> inputTransactions;
};

void processProductData(map<string, ProductData>& productMap);

int main() {
    map<string, ProductData> productMap;
    string line;

    while (getline(cin, line)) {
        istringstream ss(line);
        string productName, operation;
        double price;
        int quantity;

        getline(ss, productName, ',');
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
                int usedQuantity = min(remainingQuantity, inputQuantity);
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

    processProductData(productMap);

    close(STDIN_FILENO);
    return 0;
}

void processProductData(map<string, ProductData>& productMap) {
    double totalProfit = 0;
    int totalRemaining = 0;

    for (const auto& [productName, product] : productMap) {
        cout << "Product: " << productName
             << ", Total Profit: " << product.totalProfit
             << ", Remaining Stock: " << product.totalRemaining << endl;
        totalProfit += product.totalProfit;
        totalRemaining += product.totalRemaining;
    }

    cout << "Total Profit: " << totalProfit << endl;
    cout << "Total Remaining Stock: " << totalRemaining << endl;
}
