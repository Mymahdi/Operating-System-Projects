#include "store.h"
#include "csv.h"

Store::Store(const std::string& filePath) {
    auto rows = CSV::parse(filePath);
    for (const auto& row : rows) {
        Transaction t;
        t.product = row[0];
        t.quantity = std::stoi(row[1]);
        t.price = std::stoi(row[2]);
        t.type = row[3];
        transactions.push_back(t);
    }
}

void Store::processTransactions(std::vector<Transaction>& globalTransactions) const {
    globalTransactions.insert(globalTransactions.end(), transactions.begin(), transactions.end());
}
