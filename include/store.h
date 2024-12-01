#ifndef STORE_H
#define STORE_H

#include <string>
#include <vector>

struct Transaction {
    std::string product;
    int quantity;
    int price;
    std::string type;
};

class Store {
public:
    Store(const std::string& filePath);
    void processTransactions(std::vector<Transaction>& transactions) const;

private:
    std::vector<Transaction> transactions;
};

#endif // STORE_H
